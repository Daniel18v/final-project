<?php defined("BASEPATH") or exit("No direct script access allowed"); ?>

<?php $this->layout("templates::emulators") ?>

<?php $this->start('header') ?>
<?php $this->insert("structure::header", ['sess' => $sess]) ?>
<?php $this->stop() ?>

<?php $this->start('nav') ?>
<?php $this->insert("structure::nav") ?>
<?php $this->stop() ?>

<?php $this->start('section') ?>
<div class="container-fluid wrapper">
<!--    <h2><span class="span-night">Apagar</span><span class="span-day">Encender</span> la luz</h2>-->
    <h2><?= $rom->title ?></h2>
    <button class="day-night">
        <span class="btn-night glyph-moon-fill"></span>
        <span class="btn-day glyph-sun-fill"></span>
    </button>

    <div class="row">
        <div class="col-lg-9">
            <div id="emulator"></div>
        </div>
        <div class="col-lg-3">
            <div class="row">

                <div class="col-lg-12">
                    <div class="panel panel-info center-block">
                        <div class="panel-heading">
                            <h4 class="h4"><?= $rom->title ?></h4>
                        </div>
                        <div class="panel-body">
                            <img src="<?= $rom->image ?>" class="img-responsive center-block"/>

                            <p><?= $rom->description ?></p>
                        </div>
                    </div>


                </div>
            </div>
        </div>
    </div>
</div>
<!--<h2>Controles</h2>-->
<!--<table id="controls">-->
<!--    <tr>-->
<!--        <th>Button</th>-->
<!--        <th>Player 1</th>-->
<!--        <th>Player 2</th>-->
<!--    </tr>-->
<!--    <tr>-->
<!--        <td>Left</td>-->
<!--        <td>Left</td>-->
<!--        <td>Num-4</td>-->
<!--    <tr>-->
<!--        <td>Right</td>-->
<!--        <td>Right</td>-->
<!--        <td>Num-6</td>-->
<!--    </tr>-->
<!--    <tr>-->
<!--        <td>Up</td>-->
<!--        <td>Up</td>-->
<!--        <td>Num-8</td>-->
<!--    </tr>-->
<!--    <tr>-->
<!--        <td>Down</td>-->
<!--        <td>Down</td>-->
<!--        <td>Num-2</td>-->
<!--    </tr>-->
<!--    <tr>-->
<!--        <td>A</td>-->
<!--        <td>X</td>-->
<!--        <td>Num-7</td>-->
<!--    </tr>-->
<!--    <tr>-->
<!--        <td>B</td>-->
<!--        <td>Z/Y</td>-->
<!--        <td>Num-9</td>-->
<!--    </tr>-->
<!--    <tr>-->
<!--        <td>Start</td>-->
<!--        <td>Enter</td>-->
<!--        <td>Num-1</td>-->
<!--    </tr>-->
<!--    <tr>-->
<!--        <td>Select</td>-->
<!--        <td>Ctrl</td>-->
<!--        <td>Num-3</td>-->
<!--    </tr>-->
<!--</table>-->


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
        });

    </script>
</div>
<?php $this->stop() ?>
