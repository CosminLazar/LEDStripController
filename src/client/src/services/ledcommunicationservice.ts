import { Injectable } from '@angular/core';
import RX from 'rxjs/RX';
import { Paho } from 'ng2-mqtt/mqttws31';
import { UserSettings, IMqttServer, IControlUnit } from './usersettings';

@Injectable()
export class LedCommunicationService {
    public v = Date.now();
    private _statusUpdatesProducer = null;
    private _statusUpdates: RX.Observable<{ topic: string, state: ControlUnitState }>;
    private _client: Paho.MQTT.Client;
    private _topicSubscriptionList = new Array<string>();
    private _serializer: ControllUnitSerializer;

    constructor() {
        this._serializer = new ControllUnitSerializer();

        this._statusUpdates = RX.Observable.create(producer => {
            console.log('first subscriber');
            this._statusUpdatesProducer = producer;
            return () => { this._statusUpdatesProducer = null; };
        }).share();
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
        if (this._client && this._client.isConnected()) {
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
                    onSuccess: (a, b, c) => { resolve(); },
                    onFailure: (a, b, c) => { reject(a); }
                });
        });
    };

    private onConnectedToServer = (onLostConnection: (gracefull: boolean) => any) => {
        this._client.onMessageArrived = (a: Paho.MQTT.Message) => {
            if (this._statusUpdatesProducer) {
                let topic = a.destinationName
                let state = this._serializer.deserialize(<any>a.payloadString);

                this._statusUpdatesProducer.next({ topic: topic, state: state });
            }
        };

        this._client.onConnectionLost = (response: any) => {
            let isGracefull = response.errorCode === 0;
            onLostConnection(isGracefull);
        };

        //subscribe only once 
        new Set(this._topicSubscriptionList).forEach(x => this.hardSubscribe(x));
    };

    public subscribeToControlUnit = (unit: IControlUnit): RX.Observable<ControlUnitState> => {
        this.subscribe(unit.getStateResponseTopic);

        let filtered = this._statusUpdates
            .filter((value) => value.topic === unit.getStateResponseTopic)
            .map(x => x.state)
            .finally(() => { this.unsubscribe(unit.getStateResponseTopic); });

        return filtered;
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

        let otherSubscriptionsExist = this._topicSubscriptionList.indexOf(topic) >= 0;

        if (!otherSubscriptionsExist)
            this.hardUnsubscribe(topic);
    };

    private subscribe = (topic: string) => {
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

export class ControlUnitState {

    private _isOn: boolean;
    public get isOn(): boolean {
        return this._isOn;
    }
    public set isOn(v: boolean) {
        this._isOn = v;
    }

    private _r: number;
    public get r(): number {
        return this._r;
    }
    public set r(r: number) {
        if (r < 0 || r > 255)
            throw 'invalid value, range [0,255]';
        this._r = r;
    }

    private _g: number;
    public get g(): number {
        return this._g;
    }
    public set g(g: number) {
        if (g < 0 || g > 255)
            throw 'invalid value, range [0,255]';
        this._g = g;
    }

    private _b: number;
    public get b(): number {
        return this._b;
    }
    public set b(b: number) {
        if (b < 0 || b > 255)
            throw 'invalid value, range [0,255]';
        this._b = b;
    }


    private _brightness: number;
    public get brightness(): number {
        return this._brightness;
    }
    public set brightness(v: number) {
        if (v < 0 || v > 100)
            throw 'invalid value, range [0,100]';
        this._brightness = v;
    }
}

@Injectable()
export class ControllUnitSerializer {
    public serialize = (state: ControlUnitState): string => {
        let values = [
            state.isOn ? '1' : '0',
            state.r.toString(),
            state.g.toString(),
            state.b.toString(),
            state.brightness.toString()
        ];

        let serialized = values.join(';');

        return serialized;
    };

    public deserialize = (serialized: string): ControlUnitState => {
        let state = new ControlUnitState();

        let values = serialized.split(';');

        if (values.length < 5)
            throw 'Invalid message';

        state.isOn = values[0] === '1' ? true : false;
        state.r = Number(values[1]);
        state.g = Number(values[2])
        state.b = Number(values[3])
        state.brightness = Number(values[4])

        return state;
    };
}