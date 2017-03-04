import { Component, ViewChild } from '@angular/core';
import { NavController, Slides, Refresher, ModalController, ItemSliding } from 'ionic-angular';
import { LedController } from '../ledcontroller/ledcontroller';
import { AddNew } from '../addnew/addnew';
import { UserSettings, IControlUnit, IMqttServer } from '../../services/usersettings';
import { ServerSettingsPage } from '../serversettings/serversettings';

@Component({
  selector: 'page-home',
  templateUrl: 'home.html'
})
export class HomePage {
  constructor(
    public navCtrl: NavController, 
    private modalCtrl: ModalController, 
    public userSettings: UserSettings) {
    
  }

  public editServerSettings = () => {
    let modal = this.modalCtrl.create(ServerSettingsPage, this.userSettings.server);
    modal.onDidDismiss((result: IMqttServer) => {
      if (result) {
        this.userSettings.updateServer(result);
      }
    });
    modal.present();
  };

  public openUnit = (unit: IControlUnit) => {
    this.navCtrl.push(LedController, unit);
  };

  public addNewUnit = () => {
    var modal = this.modalCtrl.create(AddNew);
    modal.onDidDismiss((result) => {
      if (result)
        this.userSettings.addUnit(result);
    });

    modal.present();
  };

  public editUnit = (unit: IControlUnit, unitSlidingItem: ItemSliding) => {
    var modal = this.modalCtrl.create(AddNew, unit);
    modal.onDidDismiss((result: IControlUnit) => {
      unitSlidingItem.close();
      if (result) {
        this.userSettings.updateUnit(unit, result);
      }
    });

    modal.present();
  };

  public deleteUnit = (unit: IControlUnit) => {
    this.userSettings.deleteUnit(unit);
  };
}
