import { Component, ViewChild } from '@angular/core';
import { NavController, Slides, Refresher, ModalController } from 'ionic-angular';
import { LedController } from '../ledcontroller/ledcontroller';
import { AddNew } from '../addnew/addnew'
import { UserSettings, ControlUnit } from '../../services/usersettings';

@Component({
  selector: 'page-home',
  templateUrl: 'home.html'
})
export class HomePage {  
  constructor(public navCtrl: NavController, private modalCtrl: ModalController, public userSettings: UserSettings) {
    this.reload();
  }

  private reload = () => {
    //if no settings, show add new
    //if loaded from settings, navigate to the default one
  };

  public openPage = (page: any) => {
    this.navCtrl.push(page.component, page.prm);
  };

  public addNewUnit = () => {
    var modal = this.modalCtrl.create(AddNew);
    modal.onDidDismiss((result) => {
      if (result)
        this.newUnitAdded(result);
    });

    modal.present();
  };

  private newUnitAdded = (unit: ControlUnit) => {
    this.userSettings.controlUnits.push(unit);
    this.reload();
  };
}
