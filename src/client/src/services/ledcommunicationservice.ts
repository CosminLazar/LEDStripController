import { Injectable } from '@angular/core';
import RX from 'rxjs/RX';
import { Paho } from 'ng2-mqtt/mqttws31';
import { UserSettings, IMqttServer, IControlUnit } from './usersettings';

@Injectable()
export class LedCommunicationService {
    public v = Date.now();
    private _statusUpdatesProducer = null;
    public statusUpdates: RX.Observable<string>;
    private _client: Paho.MQTT.Client;
    private _topicSubscriptionList = new Array<string>();

    constructor() {
        this.statusUpdates = RX.Observable.create(producer => {
            console.log('first subscriber');
            this._statusUpdatesProducer = producer;
            return () => { this._statusUpdatesProducer = null; };
        }).share();

        setInterval(() => {
            if (this._statusUpdatesProducer)
                this._statusUpdatesProducer.next('test - ' + this.v.toString());
        }, 1000);
    }

    public connect = (server: IMqttServer, onLostConnection = (gracefull: boolean) => { }): Promise<any> => {
        return new Promise<any>((resolve, reject) => {
            if (!server) {
                reject('Server not defined');
                return;
            }

            this.doConnect(server)
                .then(() => this.onConnectedToServer(onLostConnection))
                .then(resolve, reject);
        });
    };

    public disconnect = () => {
        if (this._client.isConnected()) {
            this._client.disconnect();
        }
    };

    private doConnect = (server: IMqttServer): Promise<any> => {
        return new Promise<any>((resolve, reject) => {

            this._client = new Paho.MQTT.Client(
                server.host,
                Number(server.port),
                "visitor_" + new Date().getMilliseconds());

            this._client.connect(
                {
                    userName: server.user,
                    password: server.password,
                    useSSL: true,
                    onSuccess: (a, b, c) => { debugger; resolve(); },
                    onFailure: (a, b, c) => { debugger; reject(a); }
                });
        });
    };

    private onConnectedToServer = (onLostConnection: (gracefull: boolean) => any) => {
        this._client.onMessageArrived = (a: Paho.MQTT.Message) => {
            debugger;
            if (this._statusUpdatesProducer) {
                let payload = a.payloadString;
                this._statusUpdatesProducer.next(payload);
            }
        };

        this._client.onConnectionLost = (response: any) => {
            let isGracefull = response.errorCode === 0;
            onLostConnection(isGracefull);
        };

        this._topicSubscriptionList.forEach(x => this.hardSubscribe(x));
    };

    public subscribeToControlUnit = (unit: IControlUnit) => {
        this.subscribe(unit.getStateResponseTopic);
        //this.subscribe(unit.writeTopic);
    };

    public unsubscribeFromControlUnit = (unit: IControlUnit) => {
        this.unsubscribe(unit.getStateResponseTopic);
    };

    private unsubscribe = (topic: string) => {
        let index = this._topicSubscriptionList.indexOf(topic);
        let alreadyUnsubscribed = index < 0;

        if (alreadyUnsubscribed)
            return;

        this._topicSubscriptionList.splice(index, 1);

        this.hardUnsubscribe(topic);
    };

    private subscribe = (topic: string) => {
        let alreadySubscribed = this._topicSubscriptionList.find(x => x === topic) !== undefined;
        if (alreadySubscribed) {
            return;
        }

        this._topicSubscriptionList.push(topic);

        this.hardSubscribe(topic);
    };

    private hardSubscribe = (topic: string) => {
        if (!this._client || !this._client.isConnected())
            return;

        console.info('Subscribing to ' + topic);
        this._client.subscribe(topic, {});
    };

    private hardUnsubscribe = (topic: string) => {
        if (!this._client || !this._client.isConnected())
            return;

        console.info('Unsubscribing from ' + topic);
        this._client.unsubscribe(topic, {});
    };
}