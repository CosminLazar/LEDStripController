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
    private _serverSettings: IMqttServer;
    private _topicSubscriptionList = new Array<string>();

    constructor(settings: UserSettings) {
        this._serverSettings = settings.server;

        this._client = new Paho.MQTT.Client(
            this._serverSettings.host,
            this._serverSettings.port,
            "visitor_" + new Date().getMilliseconds());


        this._client.connect(
            {
                userName: this._serverSettings.user,
                password: this._serverSettings.password,
                useSSL: true,
                onSuccess: (a, b, c) => { debugger; this._topicSubscriptionList.forEach(x => this.hardSubscribe(x)); },
                onFailure: (a, b, c) => { debugger; }
            });

        this._client.onMessageArrived = (a: Paho.MQTT.Message) => {
            debugger;
            if (this._statusUpdatesProducer) {
                let payload = a.payloadString;
                this._statusUpdatesProducer.next(payload);
            }
        };
        this._client.onConnectionLost = (a) => { debugger; };

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

    public subscribeToControlUnit = (unit: IControlUnit) => {
        this.subscribe(unit.readTopic);
        //this.subscribe(unit.writeTopic);
    };

    public unsubscribeFromControlUnit = (unit: IControlUnit) => {
        this.unsubscribe(unit.readTopic);
    };

    private unsubscribe = (topic: string) => {
        var index = this._topicSubscriptionList.indexOf(topic);

        if (index < 0)
            return;

        this._topicSubscriptionList.splice(index, 1);

        if (this._client.isConnected()) {
            this.hardUnsubscribe(topic);
        }
    };

    private subscribe = (topic: string) => {
        let alreadySubscribed = this._topicSubscriptionList.find(x => x === topic) !== undefined;
        if (alreadySubscribed) {
            return;
        }

        this._topicSubscriptionList.push(topic);

        if (this._client.isConnected()) {
            this.hardSubscribe(topic);
        }
    };

    private hardSubscribe = (topic: string) => {
        if (!this._client.isConnected()) {
            throw 'not connected';
        }

        this._client.subscribe(topic, {});
    };

    private hardUnsubscribe = (topic: string) => {
        if (!this._client.isConnected()) {
            throw 'not connected';
        }

        this._client.unsubscribe(topic, {});
    };
}