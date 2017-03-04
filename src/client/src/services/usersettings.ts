import { Injectable } from '@angular/core';
import { Storage } from '@ionic/storage';

@Injectable()
export class UserSettings {
    public controlUnits: Array<IControlUnit>;
    public server: IMqttServer;
    private _storage: Storage;

    constructor(storage: Storage) {
        this._storage = storage;
        this.controlUnits = new Array<IControlUnit>();
        /*
        var cosmin = new IControlUnit();
        var ella = new IControlUnit();

        
                cosmin.name = 'Cosmin';
                cosmin.image = 'assets/img/cosmin_50x50.jpg';
                cosmin.readTopic = 'stl1';
                cosmin.writeTopic = 'sl1';
                ella.name = 'Ella';
                ella.image = 'assets/img/ella_50x50.jpg';
                ella.readTopic = 'stl2';
                ella.writeTopic = 'sl2';
        
                this.controlUnits.push(cosmin);
                this.controlUnits.push(ella);
        */
        this._storage.ready().then(this.loadFromStorage);
    }

    private loadFromStorage = () => {
        this._storage.get('settings').then((settings) => {
            if (!settings)
                return;

            if (settings.server) {
                this.server = settings.server;
            }

            if (settings.units && settings.units instanceof Array) {
                settings.units.forEach(x => this.controlUnits.push(x));
            }

        });
    };

    private saveToStorage = () => {
        this._storage.set(
            'settings',
            {
                units: this.controlUnits,
                server: this.server
            });
    };

    public updateServer = (serverInfo: IMqttServer) => {
        this.server = serverInfo;

        this.saveToStorage();
    };

    public addUnit = (unit: IControlUnit) => {
        this.controlUnits.push(unit);

        this.saveToStorage();
    };

    public deleteUnit = (unit: IControlUnit) => {
        let index = this.controlUnits.indexOf(unit);
        if (index >= 0) {
            this.controlUnits.splice(index, 1);
        }

        this.saveToStorage();
    };

    public updateUnit = (oldUnit: IControlUnit, newUnit: IControlUnit) => {
        oldUnit.name = newUnit.name;
        oldUnit.image = newUnit.image;
        oldUnit.readTopic = newUnit.readTopic;
        oldUnit.writeTopic = newUnit.writeTopic;

        this.saveToStorage();
    };
}

export interface IMqttServer {
    host: string;
    port: number;
    user: string;
    password: string;
}

export class IControlUnit {
    name: string;
    image: string;
    readTopic: string;
    writeTopic: string;
}