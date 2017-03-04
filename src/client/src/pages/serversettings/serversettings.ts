import { Component } from '@angular/core';
import { NavController, NavParams, ViewController } from 'ionic-angular';
import { Validators, FormBuilder, FormGroup } from '@angular/forms';
import { MqttServer } from '../../services/usersettings';

@Component({
  selector: 'page-serversettings',
  templateUrl: 'serversettings.html'
})
export class ServerSettingsPage {
  public form: FormGroup;

  constructor(
    public navCtrl: NavController,
    public navParams: NavParams,
    private formBuilder: FormBuilder,
    private viewCtrl: ViewController) {

    let initialValue = <MqttServer>(navParams.data || new MqttServer());

    this.form = this.formBuilder.group({
      host: [initialValue.host, Validators.required],
      port: [initialValue.port, Validators.required],
      user: [initialValue.user, Validators.required],
      password: [initialValue.password, Validators.required]
    });
  }

  public cancel = () => {
    this.viewCtrl.dismiss();
  };

  public save = () => {
    if (!this.form.valid)
      throw 'cannot save invalid form';

    let formResult = <MqttServer>this.form.value;
    let result = new MqttServer();

    result.host = formResult.host;
    result.port = formResult.port;
    result.user = formResult.user;
    result.password = formResult.password;

    this.viewCtrl.dismiss(result);
  };
}
