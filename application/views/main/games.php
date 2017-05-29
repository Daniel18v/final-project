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
                    <div class="col-lg-3">

                        <div class="thumbnail">
                            <img src="/images/html5/galaga.jpg"
                                 alt="Galaga">
                            <div class="caption">
                                <h3>Galaga</h3>
                                <p>Juego con navecitas y bichos raros XDDDDDD</p>
                                <p><a href="<?= site_url('juegos/galaga') ?>" class="btn btn-primary" role="button">Jugar</a>
                                </p>
                            </div>
                        </div>
                    </div>
                    <div class="col-lg-3">

                        <div class="thumbnail">
                            <img src="/images/html5/flappybird.jpg"
                                 alt="flappybird">
                            <div class="caption">
                                <h3>Flappybird</h3>
                                <p>Juego con pajaritos y tubos más complicado... xDD</p>
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
                            <img src="/images/emulators/nintendo.png"
                                 alt="NES">
                            <div class="caption">
                                <h3>NES</h3>
                                <p>Emulador de Nintendo Entertainment System</p>
                                <p>
                                    <button class="btn btm-primary <?= $sess->session->userdata('user') ? "nes" : "login" ?>"
                                            type="button"> Jugar
                                    </button>
                                </p>
                            </div>
                        </div>
                    </div>
                    <div class="col-lg-6">

                        <div class="thumbnail">
                            <img src="/images/emulators/mastersystem.png"
                                 alt="SMS">
                            <div class="caption">
                                <h3>SMS</h3>
                                <p>Emulador de Sega Master System</p>
                                <p>
                                    <button class="btn btm-primary <?= $sess->session->userdata('user') ? "sms" : "login" ?>"
                                            type="button"> Jugar
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
