import { Component, Input } from '@angular/core';
import { NavParams } from 'ionic-angular';
import { LedCommunicationService } from '../../services/ledcommunicationservice';
import { IControlUnit } from '../../services/usersettings';
import RX from 'rxjs/RX';

@Component({
    selector: 'led-controller',
    templateUrl: 'ledcontroller.html'
})
export class LedController {
    @Input() title;
    @Input() avatarImage;

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
    private _statusUpdatesSubscription: RX.Subscription;

    constructor(public commService: LedCommunicationService, navParams: NavParams) {
        this.R = 0;
        this.G = 0;
        this.B = 0;

        let settings = <IControlUnit>navParams.data;
        this.title = settings.name;
        this.avatarImage = settings.image;

        this.commService.subscribeToControlUnit(settings);

        this._statusUpdatesSubscription = this.commService.subscribeToControlUnit(settings).subscribe(st => {
            console.log(this.title + " received: " + st);
        });
    }

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
        this._statusUpdatesSubscription.unsubscribe();
    };
}
