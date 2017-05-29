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
    })
</script>
<script src="/apps/emulators/jssms/src/main.js"></script>
