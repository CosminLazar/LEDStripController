import { NgModule } from '@angular/core';
import { IonicApp, IonicModule } from 'ionic-angular';
import { MyApp } from './app.component';
import { HomePage } from '../pages/home/home';
import { LedController } from '../pages/ledcontroller/ledcontroller';
import { AddNew } from '../pages/addnew/addnew';
import { UserSettings } from '../services/usersettings';
import { LedCommunicationService } from '../services/ledcommunicationservice';

@NgModule({
  declarations: [
    MyApp,
    HomePage,
    LedController,
    AddNew
  ],
  imports: [
    IonicModule.forRoot(MyApp)
  ],
  bootstrap: [IonicApp],
  entryComponents: [    
    HomePage,
    LedController,
    AddNew
  ],
  providers: [UserSettings, LedCommunicationService]
})
export class AppModule {}
