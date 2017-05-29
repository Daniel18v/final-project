<?php defined("BASEPATH") or exit("No direct script access allowed"); ?>

<?php $this->layout("templates::games") ?>
<?php $this->start('nav') ?>
<?php $this->insert("structure::nav") ?>
<?php $this->stop() ?>
<?php $this->start('section') ?>
    <div class="row">
        <div class="col-lg-3">
        </div>
        <div class="col-lg-5">
            <div class="alert alert-success" role="alert">
                Para saltar usa la tecla space.
            </div>
        </div>
        <div class="col-lg-3">
        </div>
    </div>
    <div class="row">
        <div class="col-lg-4">
        </div>
        <div class="col-lg-4">
                <div id="bloque_juego"></div>
        </div>
        <div class="col-lg-4">
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