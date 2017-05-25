<?php defined("BASEPATH") or exit("No direct script access allowed"); ?>

<?php $this->layout("templates::games") ?>
<?php $this->start('nav') ?>
<?php $this->insert("structure::nav") ?>
<?php $this->stop() ?>
<?php $this->start('section') ?>
    <div class="container-fluid mycontent">
        <div id="flappybird" class="console">
            <div class="container-fluid text-center">
                <div class="row">
                    <div class="col-md-3 align-center">
                    </div>
                    <div class="col-md-6 align-center">
                        <div class="alert alert-success" role="alert">
                            Para saltar usa la tecla space.
                        </div>
                    </div>
                    <div class="col-md-3 align-center">
                    </div>
                </div>
                <div class="row">
                    <div class="col-md-12">
                        <div>
                            <div id="bloque_juego"></div>
                        </div>
                    </div>
                </div>
            </div>
        </div>
    </div>
    <script src="/apps/flappybird/lib/phaser.min.js"></script>
    <script src="/apps/flappybird/js/juego.js"></script>
    <script src="/apps/flappybird/js/menu.js"></script>
    <script src="/apps/flappybird/js/game-over.js"></script>
    <script src="/apps/flappybird/js/principal.js"></script>
<? $this->stop() ?>