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
                    <hr/>

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
                                    Flechas de dirección + X + Z + Intro
                                </div>
                                <div class="col-lg-6">Flechas numéricas + 7 + 9 + 1 (Pad numérico)</div>
                            </div>
                        </div>
                    </div>
                </div>
            </div>
        </div>
    </div>
    <div class="col-lg-2">
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
<script src="/apps/emulators/jssms/src/setup.js"></script>
<script src="/apps/emulators/jssms/src/sms.js"></script>
<script src="/apps/emulators/jssms/src/utils.js"></script>
<script src="/apps/emulators/jssms/src/sync-client.js"></script>
<script src="/apps/emulators/jssms/src/z80.js"></script>
<script src="/apps/emulators/jssms/src/debugger.js"></script>
<script src="/apps/emulators/jssms/src/vdp.js"></script>
<script src="/apps/emulators/jssms/src/psg.js"></script>
<script src="/apps/emulators/jssms/src/ui.js"></script>
<script src="/apps/emulators/jssms/src/keyboard.js"></script>
<script src="/apps/emulators/jssms/src/ports.js"></script>
<script src="/apps/emulators/jssms/src/compiler/bytecode.js"></script>
<script src="/apps/emulators/jssms/src/compiler/parser.js"></script>
<script src="/apps/emulators/jssms/src/compiler/opcodes-ast.js"></script>
<script src="/apps/emulators/jssms/src/compiler/opcodes-CB.js"></script>
<script src="/apps/emulators/jssms/src/compiler/opcodes-DD-FD.js"></script>
<script src="/apps/emulators/jssms/src/compiler/opcodes-ED.js"></script>
<script src="/apps/emulators/jssms/src/compiler/opcodes.js"></script>
<script src="/apps/emulators/jssms/src/compiler/analyzer.js"></script>
<script src="/apps/emulators/jssms/src/compiler/optimizer.js"></script>
<script src="/apps/emulators/jssms/src/compiler/generator.js"></script>
<script src="/apps/emulators/jssms/src/compiler/recompiler.js"></script>
<script src="/apps/emulators/jssms/lib/esprima.js"></script>
<script src="/apps/emulators/jssms/lib/escodegen.browser.js"></script>
<script>
    $(function () {
        var sms = new JSSMS({
            'ui': $('#emulator').JSSMSUI({'homebrew': '<?= $rom->rom ?>'})
        });
        $('#gamepad').remove();
        $('#close').on('click', function() {
            $(".panel-body").toggle();
        })
    })
</script>
<script src="/apps/emulators/jssms/src/main.js"></script>
<?php $this->stop() ?>
