import { Component, ViewChild } from '@angular/core';
import { NavController, Slides, Refresher } from 'ionic-angular';
import { LedController } from '../ledcontroller/ledcontroller';

@Component({
  selector: 'page-home',
  templateUrl: 'home.html'
})
export class HomePage {
  @ViewChild('firstController') firstController: LedController;
  @ViewChild('secondController') secondController: LedController;

  @ViewChild('controllerSlider') slider: Slides;

  constructor(public navCtrl: NavController) {

  }

  public refreshController = (refresher: Refresher) => {
    this.getActiveController().refresh().then(() => {
      refresher.complete();
    });
  };

  private getActiveController = () => {
    if (this.slider.getActiveIndex() == 0)
      return this.firstController;
    return this.secondController;
  };
}
