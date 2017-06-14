<?php defined("BASEPATH") or exit("No direct script access allowed"); ?>

<?php $this->layout("templates::master") ?>
<?php $this->start('header') ?>
<?php $this->insert("structure::header", ['sess' => $sess]) ?>
<?php $this->stop() ?>

<?php $this->start('nav') ?>
<?php $this->insert("structure::nav") ?>
<?php $this->stop() ?>

<?php $this->start('section') ?>
    <div class="container">
        <div class="row">
            <div class="col-xs-12 col-sm-12 col-md-6 col-lg-6 col-xs-offset-0 col-sm-offset-0 col-md-offset-3 col-lg-offset-3 toppad" >
                <div class="panel panel-info">
                    <div class="panel-heading">
                        <h3 class="panel-title"><?= $user['user'] ?></h3>
                    </div>
                    <div class="panel-body">
                        <div class="row">
                            <div class="col-md-3 col-lg-3 " align="center"> <img alt="User Pic" src="<?= $user['avatar'] ?>" class="img-circle img-responsive"> </div>
                            <div class=" col-md-9 col-lg-9 ">
                                <table class="table table-user-information">
                                    <tbody>
                                    <tr>
                                        <td>Birds coins:</td>
                                        <td> <?= $user['bird_coins'] ?> <span class="glyphicon glyphicon-piggy-bank"></span></td>
                                    </tr>
                                    <tr>
                                        <td>Nombre</td>
                                        <td><?= $user['name'] ?></td>
                                    </tr>
                                    <tr>
                                        <td>Primer apellido</td>
                                        <td><?= $user['first_last'] ?></td>
                                    </tr>
                                    <tr>
                                        <td>Segundo apellido</td>
                                        <td><?= $user['second_last'] ?></td>
                                    </tr>
                                    <tr>
                                        <td>Fecha de nacimiento</td>
                                        <td><?= date('d-m-Y', strtotime($user['bornDate']))  ?></td>
                                    </tr>
                                    <tr>
                                        <td>Fecha de registro:</td>
                                        <td> <?= date('d-m-Y', strtotime($user['hire'])) ?> </td>
                                    </tr>
                                    <tr>
                                        <td>Género:</td>
                                        <td><?= $user['gender'] ?></td>
                                    </tr>


                                    <tr>
                                        <td>Correo electrónico</td>
                                        <td><a href="mailto:<?= $user['mail']?>"> <?=$user['mail'] ?> </a></td>
                                    </tr>
                                    </tbody>
                                </table>

                            </div>


                        </div>
                    </div>


                </div>
            </div>
        </div>
    </div>
<?php $this->stop() ?>