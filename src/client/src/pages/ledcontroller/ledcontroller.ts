import { Component } from '@angular/core';

@Component({
    selector: 'led-controller',
    templateUrl: 'ledcontroller.html'
})
export class LedController {
    name = 'ella';
    public R = 0;
    public G = 0;
    public B = 0;

    public getCssColor(): string {
        return 'rgb(' + this.R + ',' + this.G + ',' + this.B + ')';
    }

    public refresh = () => {
        return new Promise((resolve, reject) => {
            setTimeout(function() {
                resolve();
            }, 2000);
        });
    };
}
