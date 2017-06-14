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
    <div id="galaga" class="console">
        <div class="container-fluid text-center">
            <div class="row">
                <div class="col-lg-4 col-sm-1">
                </div>
                <div class="col-lg-4 col-sm-10">
                    <div class="alert alert-success alert-dismissable fade in" role="alert">
                        <a href="#" class="close" data-dismiss="alert" aria-label="close">&times;</a>
                        Pulsa 1 para un jugador, 2 para dos jugadores.<br/>

                        Para moverse usar las teclas de dirección y Ctrl para disparar.
                    </div>
                </div>
                <div class="col-lg-4 col-sm-1">
                </div>
            </div>
            <div class="row">
                <div class="col-lg-4 col-sm-1">

                </div>
                <div class="col-lg-4 col-sm-10">
                    <div>
                        <canvas id="myGLCanvas"/>
                    </div>
                </div>
                <div class="col-lg-4 col-sm-1">

                </div>
            </div>
        </div>
    </div>
</div>

<script src="/apps/galaga/WebApplication/galaga.js"></script>
<script src="/apps/galaga/WebApplication/game.js"></script>
<? $this->stop() ?>


