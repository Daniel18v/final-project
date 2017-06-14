<?php defined("BASEPATH") or exit("No direct script access allowed"); ?>

<?php $this->layout("templates::master") ?>

<?php $this->start('header') ?>
<?php $this->insert("structure::header", ['sess' => $sess]) ?>
<?php $this->stop() ?>

<?php $this->start('nav') ?>
<?php $this->insert("structure::nav") ?>
<?php $this->stop() ?>

<?php $this->start('section') ?>
<div class="row">
    <div class="col-lg-6">
        <div class="panel panel-info">
            <div class="panel-heading">
                <h3 class="panel-title">Juegos HTML5</h3>
            </div>
            <div class="panel-body">
                <div class="row text-center">
                    <div class="col-lg-6">

                        <div class="thumbnail">
                            <img src="/images/html5/galaga.jpg"
                                 alt="Galaga">
                            <div class="caption">
                                <h3>Galaga</h3>
                                <p>Clásico juego de naves. La nostalgia recorrerá tu cuerpo al jugar a este juego.</p>
                                <p><a href="<?= site_url('juegos/galaga') ?>" class="btn btn-primary" role="button">Jugar</a>
                                </p>
                            </div>
                        </div>
                    </div>
                    <div class="col-lg-6">

                        <div class="thumbnail">
                            <img src="/images/html5/flappybird.jpg"
                                 alt="flappybird">
                            <div class="caption">
                                <h3>Flappybird</h3>
                                <p>La joya de la corona de FreeBird. Aquí podrás conseguir bird-coins siempre que estés registrado.
                                ¿A qué esperas?</p>
                                <p><a href="<?= site_url('juegos/flappybird') ?>" class="btn btn-primary" role="button">Jugar</a>
                                </p>
                            </div>
                        </div>
                    </div>
                </div>
            </div>

        </div>
    </div>
    <div class="col-lg-6">
        <div class="panel panel-info">
            <div class="panel-heading">
                <h3 class="panel-title">Emuladores</h3>
            </div>
            <div class="panel-body">
                <div class="row text-center">
                    <div class="col-lg-6">

                        <div class="thumbnail">
                            <img class="<?= $sess->session->userdata('user') ? "nes" : "login" ?>" src="/images/emulators/nintendo.png"
                                 alt="NES">
                            <div class="caption">
                                <p>
                                    <button class="btn btn<?= $sess->session->userdata('user') ? "-success nes" : "-danger login" ?>"
                                            type="button"> Acceder
                                    </button>
                                </p>
                            </div>
                        </div>
                    </div>
                    <div class="col-lg-6">

                        <div class="thumbnail">
                            <img class="<?= $sess->session->userdata('user') ? "sms" : "login" ?>" src="/images/emulators/mastersystem.png"
                                 alt="SMS">
                            <div class="caption">
                                 <p>
                                    <button class="btn btn<?= $sess->session->userdata('user') ? "-success sms" : "-danger login" ?>"
                                            type="button"> Acceder
                                    </button>
                                </p>
                            </div>
                        </div>
                    </div>

                </div>
            </div>
        </div>
    </div>
</div>

<?php $this->stop() ?>
