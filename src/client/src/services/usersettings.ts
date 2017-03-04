import { Injectable } from '@angular/core';

@Injectable()
export class UserSettings {
    public controlUnits: Array<ControlUnit>;
    public server: MqttServer;

    constructor() {
        this.controlUnits = new Array<ControlUnit>();
        var cosmin = new ControlUnit();
        var ella = new ControlUnit();

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
    }

    public updateServer = (serverInfo: MqttServer) => {
        this.server = serverInfo;
    };

    public addUnit = (unit: ControlUnit) => {
        this.controlUnits.push(unit);
    };

    public deleteUnit = (unit: ControlUnit) => {
        let index = this.controlUnits.indexOf(unit);
        if (index >= 0) {
            this.controlUnits.splice(index, 1);
        }
    };

    public updateUnit = (oldUnit: ControlUnit, newUnit: ControlUnit) => {
        oldUnit.name = newUnit.name;
        oldUnit.image = newUnit.image;
        oldUnit.readTopic = newUnit.readTopic;
        oldUnit.writeTopic = newUnit.writeTopic;
    };
}

export class MqttServer {
    public host: string;
    public port: number;
    public user: string;
    public password: string;
}

export class ControlUnit {
    public name: string;
    public image: string;
    public readTopic: string;
    public writeTopic: string;
    //other topics, e.g.: results
}