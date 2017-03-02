import { Component, ViewChild } from '@angular/core';
import { StatusBar } from 'ionic-native';
import { Platform, MenuController, Nav } from 'ionic-angular';
import { HomePage } from '../pages/home/home';
import { AddNew } from '../pages/addnew/addnew'
import { LedController } from '../pages/ledcontroller/ledcontroller';
import { UserSettings, ControlUnit } from '../services/usersettings';

@Component({
  templateUrl: 'app.html'
})
export class MyApp {
  rootPage = HomePage; 
  
  constructor(platform: Platform) {
    platform.ready().then(() => {
      // Okay, so the platform is ready and our plugins are available.
      // Here you can do any higher level native things you might need.
      StatusBar.styleDefault();     
    });
  }
}
