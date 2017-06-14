<?php defined("BASEPATH") or exit("No direct script access allowed"); ?>

<?php $this->layout("templates::emulators") ?>

<?php $this->start('header') ?>
<?php $this->insert("structure::header", ['sess' => $sess]) ?>
<?php $this->stop() ?>

<?php $this->start('nav') ?>
<?php $this->insert("structure::nav") ?>
<?php $this->stop() ?>

<?php $this->start('section') ?>
<h2><?= $rom->title ?></h2>
<button class="day-night">
    <span class="btn-night glyph-moon-fill"></span>
    <span class="btn-day glyph-sun-fill"></span>
</button>

<div class="row">
    <div class="col-lg-1">
    </div>
    <div class="col-lg-7">
        <div id="emulator"></div>
        <div class="row">
            <div class="col-lg-6 col-lg-offset-3 hr-margin-top-10">
                <div class="alert alert-success alert-dismissable fade in" role="alert">
                    <a href="#" class="close" data-dismiss="alert" aria-label="close">&times;</a>
                    <h4>Controles</h4>
                    <hr />
                    <div class="row">
                        <div class="col-lg-12">
                            <div class="row">
                                <div class="col-lg-6">
                                    Jugador 1
                                </div>
                                <div class="col-lg-6">
                                    Jugador 2
                                </div>
                            </div>
                        </div>
                    </div>
                    <div class="row">
                        <div class="col-lg-12">
                            <div class="row">
                                <div class="col-lg-6">
                                    Flechas de dirección + A + B + Intro + Ctrl
                                </div>
                                <div class="col-lg-6">Flechas numéricas + Z + Y + 1 + 3 (Pad numérico)</div>
                            </div>
                        </div>
                    </div>
                </div>
            </div>
        </div>
    </div>
    <div class="col-lg-2" id="panel">
        <div class="row">

            <div class="col-lg-12">
                <div class="panel panel-info center-block">
                    <div class="panel-heading"  id="panel">

                        <h4 class="h4"><?= $rom->title ?>
                            <button type="button" class="close" id="close"  data-dismiss="alert"><span
                                        aria-hidden="true">×</span><span class="sr-only">Cerrar</span>
                        </h4>
                    </div>
                    <div class="panel-body">
                        <img src="<?= explode(',', $rom->image)[0] ?>" class="img-responsive center-block"/>

                        <p><?= $rom->description ?></p>
                    </div>
                </div>
            </div>
        </div>
    </div>
    <div class="col-lg-2">
    </div>
</div>
<?php $this->stop() ?>
<?php $this->start('scripts') ?>
<div>
    <script src="/apps/emulators/jsnes/lib/jquery-1.4.2.min.js" type="text/javascript" charset="utf-8"></script>
    <script src="/apps/emulators/jsnes/lib/dynamicaudio-min.js" type="text/javascript" charset="utf-8"></script>
    <script src="/apps/emulators/jsnes/source/nes.js" type="text/javascript" charset="utf-8"></script>
    <script src="/apps/emulators/jsnes/source/utils.js" type="text/javascript" charset="utf-8"></script>
    <script src="/apps/emulators/jsnes/source/cpu.js" type="text/javascript" charset="utf-8"></script>
    <script src="/apps/emulators/jsnes/source/keyboard.js" type="text/javascript" charset="utf-8"></script>
    <script src="/apps/emulators/jsnes/source/mappers.js" type="text/javascript" charset="utf-8"></script>
    <script src="/apps/emulators/jsnes/source/papu.js" type="text/javascript" charset="utf-8"></script>
    <script src="/apps/emulators/jsnes/source/ppu.js" type="text/javascript" charset="utf-8"></script>
    <script src="/apps/emulators/jsnes/source/rom.js" type="text/javascript" charset="utf-8"></script>
    <script src="/apps/emulators/jsnes/source/ui.js" type="text/javascript" charset="utf-8"></script>
    <script type="text/javascript" charset="utf-8">
        $(function () {
            var nes = new JSNES({
                'ui': $('#emulator').JSNESUI({"homebrew": '<?= $rom->rom ?>'})
            });
            $('#close').click(function() {
                $(".panel-body").toggle();
            });
        });

    </script>
</div>
<?php $this->stop() ?>
