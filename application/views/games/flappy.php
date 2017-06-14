<?php defined("BASEPATH") or exit("No direct script access allowed"); ?>

<?php $this->layout("templates::games") ?>
<?php $this->start('header') ?>
<?php $this->insert("structure::header", ['sess' => $sess]) ?>
<?php $this->stop() ?>
<?php $this->start('nav') ?>
<?php $this->insert("structure::nav") ?>
<?php $this->stop() ?>
<?php $this->start('section') ?>
    <div class="container-fluid mycontent">
        <div class="row text-center">
            <div class="col-lg-4 col-sm-1">
            </div>
            <div class="col-lg-4 col-sm-10">
                <div class="alert alert-success alert-dismissable fade in" role="alert">
                    <a href="#" class="close" data-dismiss="alert" aria-label="close">&times;</a>
                    Para saltar usa la tecla espacio.
                </div>
            </div>
            <div class="col-lg-4 col-sm-1">
            </div>
        </div>
        <div class="row">
            <div class="col-lg-4 col-sm-1">
            </div>
            <div class="col-lg-4 col-sm-10">
                <div id="bloque_juego"></div>
            </div>
            <div class="col-lg-4 col-sm-1">
            </div>
        </div>

    </div>
<?php $this->stop() ?>
<?php $this->start('scripts') ?>
    <script src="/apps/flappybird/lib/phaser.min.js"></script>
    <script src="/apps/flappybird/js/juego.js"></script>
    <script src="/apps/flappybird/js/menu.js"></script>
    <script src="/apps/flappybird/js/game-over.js"></script>
    <script src="/apps/flappybird/js/principal.js"></script>
<?php $this->stop() ?>