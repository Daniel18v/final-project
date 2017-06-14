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
                        <h3 class="panel-title"><?= $sess->session->userdata('user')?></h3>
                    </div>
                    <div class="panel-body">
                        <div class="row">
                            <div class="col-md-3 col-lg-3 " align="center"> <img alt="User Pic" src="<?= $sess->session->userdata('avatar')?>" class="img-circle img-responsive"> </div>
                            <div class=" col-md-9 col-lg-9 ">
                                <table class="table table-user-information">
                                    <tbody>
                                    <tr>
                                        <td>Birds coins:</td>
                                        <td> <?= $sess->session->userdata('bird_coins')?> <span class="glyphicon glyphicon-piggy-bank"></span></td>
                                    </tr>
                                    <tr>
                                        <td>Nombre</td>
                                        <td><?= $sess->session->userdata('name')?></td>
                                    </tr>
                                    <tr>
                                        <td>Primer apellido</td>
                                        <td><?= $sess->session->userdata('first_last')?></td>
                                    </tr>
                                    <tr>
                                        <td>Segundo apellido</td>
                                        <td><?= $sess->session->userdata('second_last')?></td>
                                    </tr>
                                    <tr>
                                        <td>Fecha de nacimiento</td>
                                        <td><?= date('d-m-Y', strtotime($sess->session->userdata('bornDate'))) ?></td>
                                    </tr>
                                    <tr>
                                        <td>Fecha de registro:</td>
                                        <td> <?= date('d-m-Y', strtotime($sess->session->userdata('hire')))?> </td>
                                    </tr>
                                    <tr>
                                        <td>Género:</td>
                                        <td><?= $sess->session->userdata('gender')?></td>
                                    </tr>


                                    <tr>
                                        <td>Correo electrónico</td>
                                        <td><a href="mailto:<?= $sess->session->userdata('mail')?>"> <?=$sess->session->userdata('mail') ?> </a></td>
                                    </tr>
                                    </tbody>
                                </table>

                            </div>


                        </div>
                    </div>
                    <div class="panel-footer">
                        <a href="perfil/mis-juegos"
                           data-original-title="Ver mis juegos" data-toggle="tooltip" type="button"
                           class="btn btn-sm btn-primary">Mis juegos <i class="fa fa-gamepad" aria-hidden="true"></i></a>
                        <span class="pull-right">
                            <a href="/perfil/editar" data-original-title="Editar usuario" data-toggle="tooltip" type="button" class="btn btn-sm btn-warning">Editar <i class="glyphicon glyphicon-edit"></i></a>
                        </span>
                    </div>

                </div>
            </div>
        </div>
    </div>
<?php $this->stop() ?>