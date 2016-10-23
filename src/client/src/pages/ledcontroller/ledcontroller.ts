import { Component, Input } from '@angular/core';

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

    constructor() {
        this.R = 0;
        this.G = 0;
        this.B = 0;
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
}
