<?php defined("BASEPATH") or exit("No direct script access allowed"); ?>

<?php $this->layout("templates::games") ?>
<?php $this->start('nav') ?>
<?php $this->insert("structure::nav") ?>
<?php $this->stop() ?>
<?php $this->start('section') ?>
<div class="container-fluid mycontent">
    <div id="galaga" class="console">
        <div class="container-fluid text-center">
            <div class="row">
                <div class="col-md-3 align-center">
                </div>
                <div class="col-md-6 align-center">
                    <div class="alert alert-success" role="alert">
                        Pulsa 1 para un jugador, 2 para dos jugadores.<br/>

                        Para moverse usar las teclas de dirección y Ctrl para disparar.
                    </div>
                </div>
                <div class="col-md-3 align-center">
                </div>
            </div>
            <div class="row">
                <div class="col-md-12">
                    <div>
                        <canvas id="myGLCanvas"/>
                    </div>
                </div>
            </div>
        </div>
    </div>
</div>

<script src="/apps/galaga/WebApplication/galaga.js"></script>
<script src="/apps/galaga/WebApplication/game.js"></script>
<? $this->stop() ?>


