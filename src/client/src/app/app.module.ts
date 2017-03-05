import { NgModule } from '@angular/core';
import { IonicApp, IonicModule } from 'ionic-angular';
import { MyApp } from './app.component';
import { HomePage } from '../pages/home/home';
import { LedController } from '../pages/ledcontroller/ledcontroller';
import { AddNew } from '../pages/addnew/addnew';
import { UserSettings } from '../services/usersettings';
import { LedCommunicationService, ControllUnitSerializer } from '../services/ledcommunicationservice';
import { ServerSettingsPage } from '../pages/serversettings/serversettings';
import { Storage } from '@ionic/storage';

@NgModule({
  declarations: [
    MyApp,
    HomePage,
    LedController,
    AddNew,
    ServerSettingsPage
  ],
  imports: [
    IonicModule.forRoot(MyApp)
  ],
  bootstrap: [IonicApp],
  entryComponents: [    
    HomePage,
    LedController,
    AddNew,
    ServerSettingsPage
  ],
  providers: [UserSettings, LedCommunicationService, Storage, ControllUnitSerializer]
})
export class AppModule {}
