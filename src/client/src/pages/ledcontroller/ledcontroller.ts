import { Component, Input } from '@angular/core';
import { NavParams } from 'ionic-angular';
import { LedCommunicationService, ControlUnitState } from '../../services/ledcommunicationservice';
import { IControlUnit } from '../../services/usersettings';
import RX from 'rxjs/RX';

@Component({
    selector: 'led-controller',
    templateUrl: 'ledcontroller.html'
})
export class LedController {
    @Input() title;
    @Input() avatarImage;


    private _brightness: number;
    public get brightness(): number {
        return this._brightness;
    }
    public set brightness(v: number) {
        this._brightness = v;
    }

    private _hasPower: boolean;
    public get hasPower(): boolean {
        return this._hasPower;
    }
    public set hasPower(v: boolean) {
        this._hasPower = v;
    }

    private _R: number;
    public get R(): number {
        return this._R;
    }
    public set R(v: number) {
        this._R = v;
        this.updateRgbPreview();
    }

    private _G: number;
    public get G(): number {
        return this._G;
    }
    public set G(v: number) {
        this._G = v;
        this.updateRgbPreview();
    }

    private _B: number;
    public get B(): number {
        return this._B;
    }
    public set B(v: number) {
        this._B = v;
        this.updateRgbPreview();
    }

    public rgbPreview: string;
    private _unitSubscription: RX.Subscription;

    constructor(public commService: LedCommunicationService, navParams: NavParams) {
        this.hasPower = false;
        this.brightness = 0;
        this.R = 0;
        this.G = 0;
        this.B = 0;

        let settings = <IControlUnit>navParams.data;
        this.title = settings.name;
        this.avatarImage = settings.image;

        this._unitSubscription = this.commService.createUnitSubscription(settings).subscribe(st => {
            this.updateFromRemoteState(st);
        });
    }

    private updateFromRemoteState = (remoteState: ControlUnitState) => {
        this.hasPower = remoteState.isOn;
        this.brightness = remoteState.brightness;
        this.R = remoteState.r;
        this.G = remoteState.g;
        this.B = remoteState.b;
    };

    public refresh = () => {
        return new Promise((resolve, reject) => {
            setTimeout(function () {
                resolve();
            }, 2000);
        });
    };

    private updateRgbPreview = (): void => {
        this.rgbPreview = 'rgb(' + this.R + ',' + this.G + ',' + this.B + ')';
    };

    public ionViewWillUnload = () => {
        this._unitSubscription.unsubscribe();
    };
}
