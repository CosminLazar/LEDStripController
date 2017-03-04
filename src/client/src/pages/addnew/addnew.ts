import { NavController, Slides, Refresher, ViewController } from 'ionic-angular';
import { Component } from '@angular/core';
import { Validators, FormBuilder, FormGroup } from '@angular/forms';
import { IControlUnit } from '../../services/usersettings';

@Component({
  selector: 'add-new',
  templateUrl: 'addnew.html'
})
export class AddNew {
  public avatars: Array<string>;
  public form: FormGroup;

  constructor(private viewCtrl: ViewController, private formBuilder: FormBuilder) {
    this.avatars = [
      'assets/img/ella_50x50.jpg',
      'assets/img/cosmin_50x50.jpg'
    ];

    let initialValue = <IControlUnit>(viewCtrl.data || {});

    this.form = this.formBuilder.group({
      name: [initialValue.name, Validators.required],
      readTopic: [initialValue.readTopic, Validators.required],
      writeTopic: [initialValue.writeTopic, Validators.required],
      avatar: [initialValue.image || this.avatars[0], Validators.required]
    });
  }

  public selectAvatar = (avatarUrl) => {
    this.form.patchValue({ avatar: avatarUrl });
  };

  public isAvatarSelected = (avatarUrl): boolean => {
    return this.form.get('avatar').value === avatarUrl;
  };

  public cancel = () => {
    this.viewCtrl.dismiss();
  };

  public save = () => {
    if (!this.form.valid)
      throw 'cannot save invalid form';

    let formResult = this.form.value;
    let result: IControlUnit = {
      name: formResult.name,
      image: formResult.avatar,
      readTopic: formResult.readTopic,
      writeTopic: formResult.writeTopic
    };

    this.viewCtrl.dismiss(result);
  };
}