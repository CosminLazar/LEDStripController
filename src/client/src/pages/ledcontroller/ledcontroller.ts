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


    private _brightness: number = 0;
    public get brightness(): number {
        return this._brightness;
    }
    public set brightness(v: number) {
        this._brightness = v;
        this.stateChanged();
    }

    private _hasPower: boolean = false;
    public get hasPower(): boolean {
        return this._hasPower;
    }
    public set hasPower(v: boolean) {
        this._hasPower = v;
        this.stateChanged();
    }

    private _R: number = 0;
    public get R(): number {
        return this._R;
    }
    public set R(v: number) {
        this._R = v;
        this.updateRgbPreview();
        this.stateChanged();
    }

    private _G: number = 0;
    public get G(): number {
        return this._G;
    }
    public set G(v: number) {
        this._G = v;
        this.updateRgbPreview();
        this.stateChanged();
    }

    private _B: number = 0;
    public get B(): number {
        return this._B;
    }
    public set B(v: number) {
        this._B = v;
        this.updateRgbPreview();
        this.stateChanged();
    }

    public rgbPreview: string;
    private _unitSubscription: RX.Subscription;
    private _unit: IControlUnit;
    private _shouldReportStateChanges: boolean = true;

    constructor(public commService: LedCommunicationService, navParams: NavParams) {
        this._unit = <IControlUnit>navParams.data;
        this.title = this._unit.name;
        this.avatarImage = this._unit.image;
    }

    private ionViewDidLoad = () => {
        this.updateRgbPreview();
        this._unitSubscription = this.commService.createUnitSubscription(this._unit).subscribe(st => {
            this.updateFromRemoteState(st);
        });
    };

    private updateFromRemoteState = (remoteState: ControlUnitState) => {
        this.doInNonStateReportingContext(() => {
            this.hasPower = remoteState.isOn;
            this.brightness = remoteState.brightness;
            this.R = remoteState.r;
            this.G = remoteState.g;
            this.B = remoteState.b;
        });
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

    private doInNonStateReportingContext = (action: () => any) => {
        this._shouldReportStateChanges = false;
        action();
        this._shouldReportStateChanges = true;
    };

    private stateChanged = () => {
        if (!this._shouldReportStateChanges)
            return;

        let state = new ControlUnitState();
        state.isOn = this.hasPower;
        state.brightness = this.brightness;
        state.r = this.R;
        state.g = this.G;
        state.b = this.B;
        this.commService.setUnitState(this._unit, state);
    };
}
