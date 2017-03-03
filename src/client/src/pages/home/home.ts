import { Component, ViewChild } from '@angular/core';
import { NavController, Slides, Refresher, ModalController, ItemSliding } from 'ionic-angular';
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

  public openUnit = (unit: ControlUnit) => {
    this.navCtrl.push(LedController, unit);
  };

  public addNewUnit = () => {
    var modal = this.modalCtrl.create(AddNew);
    modal.onDidDismiss((result) => {
      if (result)
        this.newUnitAdded(result);
    });

    modal.present();
  };

  public editUnit = (unit: ControlUnit, unitSlidingItem: ItemSliding) => {
    var modal = this.modalCtrl.create(AddNew, unit);
    modal.onDidDismiss((result: ControlUnit) => {
      unitSlidingItem.close();

      if (result) {
        unit.name = result.name;
        unit.image = result.image;
        unit.readTopic = result.readTopic;
        unit.writeTopic = result.writeTopic;
      }
    });

    modal.present();

  };

  private newUnitAdded = (unit: ControlUnit) => {
    this.userSettings.controlUnits.push(unit);
    this.reload();
  };
}
