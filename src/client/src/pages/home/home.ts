import { Component, ViewChild } from '@angular/core';
import { NavController, Slides, Refresher, ModalController, ItemSliding } from 'ionic-angular';
import { LedController } from '../ledcontroller/ledcontroller';
import { AddNew } from '../addnew/addnew';
import { UserSettings, IControlUnit, IMqttServer } from '../../services/usersettings';
import { ServerSettingsPage } from '../serversettings/serversettings';
import { LedCommunicationService } from '../../services/ledcommunicationservice';

@Component({
  selector: 'page-home',
  templateUrl: 'home.html'
})
export class HomePage {
  constructor(
    public navCtrl: NavController,
    private modalCtrl: ModalController,
    public userSettings: UserSettings,
    private communicationService: LedCommunicationService) {
  }

  private ionViewDidLoad = () => {
    this.userSettings.doneLoading().then(() => {
      if (this.userSettings.server) {
        this.connect(this.userSettings.server);
      }
    });
  };

  private connect = (server: IMqttServer): Promise<any> => {
    return this.communicationService.connect(this.userSettings.server, this.onLostConnection)
      .then(
      () => {
        console.log('connected');
        //subscribe
        //todo, get a hold of subscription
        //this.userSettings.controlUnits.forEach(x => this.communicationService.subscribeToControlUnit(x));
        //request status for each
      },
      () => { console.log('could not connect'); });
  };

  private onLostConnection = (gracefull: boolean) => {
    if (gracefull)
      console.log('terminated connection');
    else
      console.log('lost connection, do something');
  };

  public editServerSettings = () => {
    let modal = this.modalCtrl.create(ServerSettingsPage, this.userSettings.server);
    modal.onDidDismiss((result: IMqttServer) => {
      if (result) {
        this.communicationService.disconnect();
        this.userSettings.updateServer(result);
        this.connect(this.userSettings.server);
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
      if (result) {
        this.userSettings.addUnit(result);
        this.communicationService.createUnitSubscription(result);
      }
    });

    modal.present();
  };

  public editUnit = (unit: IControlUnit, unitSlidingItem: ItemSliding) => {
    var modal = this.modalCtrl.create(AddNew, unit);
    modal.onDidDismiss((result: IControlUnit) => {
      unitSlidingItem.close();
      if (result) {
        this.communicationService.unsubscribeFromControlUnit(unit);
        this.userSettings.updateUnit(unit, result);
        this.communicationService.createUnitSubscription(result);
      }
    });

    modal.present();
  };

  public deleteUnit = (unit: IControlUnit) => {
    this.communicationService.unsubscribeFromControlUnit(unit);
    this.userSettings.deleteUnit(unit);
  };
}
