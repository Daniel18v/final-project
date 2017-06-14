<?php defined("BASEPATH") or exit("No direct script access allowed"); ?>

<?php $this->layout("templates::master") ?>
<?php $this->start('header') ?>
<?php $this->insert("structure::header", ['sess' => $sess]) ?>
<?php $this->stop() ?>

<?php $this->start('nav') ?>
<?php $this->insert("structure::nav") ?>
<?php $this->stop() ?>

<?php $this->start('section') ?>

    <form action="<?= site_url() . "perfil/actualizar" ?>" method="post">
        <div class="container">
            <div class="row">
                <div class="col-xs-12 col-sm-12 col-md-6 col-lg-6 col-xs-offset-0 col-sm-offset-0 col-md-offset-3 col-lg-offset-3 toppad">

                    <div class="panel panel-info">
                        <div class="panel-heading">
                            <h3 class="panel-title"><?= $sess->session->userdata('user') ?></h3>
                        </div>
                        <div class="panel-body">
                            <div class="row">
                                <div class="col-md-3 col-lg-3 " align="center"><img alt="Imagen de usuario"
                                                                                    src="<?= $sess->session->userdata('avatar') ?>"
                                                                                    class="img-circle img-responsive">
                                </div>
                                <div class=" col-md-9 col-lg-9 ">
                                    <table class="table table-user-information">
                                        <tbody>
                                        <?php if ($sess->session->userdata('rol') == 1) : ?>
                                            <tr>
                                                <td>Birds coins:</td>
                                                <td><input type="number"
                                                           name="bird_coins"
                                                           value="<?= $sess->session->userdata('bird_coins') ?>"/> <span
                                                            class="glyphicon glyphicon-piggy-bank"></span></td>
                                            </tr>
                                        <?php endif ?>
                                        <tr>
                                            <td>Nombre</td>
                                            <td><input type="text" name="name"
                                                       value="<?= $sess->session->userdata('name') ?>"/>
                                            </td>
                                        </tr>
                                        <tr>
                                            <td>Primer apellido</td>
                                            <td><input type="text" name="first_last"
                                                       value="<?= $sess->session->userdata('first_last') ?>"/></td>
                                        </tr>
                                        <tr>
                                            <td>Segundo apellido</td>
                                            <td><input type="text" name="second_last"
                                                       value="<?= $sess->session->userdata('second_last') ?>"/></td>
                                        </tr>
                                        <tr>
                                            <td>Fecha de nacimiento</td>
                                            <td><input type="date" name="bornDate"
                                                       value="<?= date('m-d-Y', $sess->session->userdata('bornData')); ?>"/>
                                            </td>
                                        </tr>
                                        <tr>
                                            <td>Género:</td>
                                            <td>

                                                <input type="radio" name="gender"
                                                       value="Hombre" <?= ($sess->session->userdata('gender') !=
                                                    "Hombre") ? : "checked" ?>> Hombre<br>
                                                <input type="radio" name="gender"
                                                       value="Mujer" <?= ($sess->session->userdata('gender') !=
                                                    "Mujer") ? : "checked" ?>> Mujer<br>
                                                <input type="radio" name="gender"
                                                       value="Otro" <?= ($sess->session->userdata('gender') !=
                                                    "Otro") ? : "checked" ?>> Otro
                                            </td>
                                        </tr>
                                        <tr>
                                            <td>Correo electrónico</td>
                                            <td>
                                                <?= "<input type='text' class='hide' name='mail' value='" .
                                                $sess->session->userdata('mail') . "'>" .
                                                $sess->session->userdata('mail') ?>
                                            </td>
                                        </tr>

                                        </tbody>
                                    </table>

                                </div>


                            </div>
                        </div>
                        <div class="panel-footer">
                            <a href="mis-juegos"
                               data-original-title="Ver mis juegos" data-toggle="tooltip" type="button"
                               class="btn btn-sm btn-primary">Mis juegos <i class="fa fa-gamepad" aria-hidden="true"></i></a>
                            <span class="pull-right">
                                <button type="submit" data-original-title="Editar usuario"
                                        class="btn btn-sm btn-warning">Editar <i class="glyphicon glyphicon-edit"></i></button>

                        </span>
                        </div>

                    </div>
                </div>
            </div>
        </div>
    </form>


<?php $this->stop() ?>