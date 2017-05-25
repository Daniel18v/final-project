<?php defined("BASEPATH") or exit("No direct script access allowed"); ?>

<?php $this->layout("templates::games") ?>

<?php $this->start('header') ?>
<?php $this->insert("structure::header") ?>
<?php $this->stop() ?>

<?php $this->start('nav') ?>
<?php $this->insert("structure::nav") ?>
<?php $this->stop() ?>

<?php $this->start('section') ?>
    <div class="container-fluid mycontent">
        <head>
            <style>
                body {
                    /* background: #000;*/
                }

                /* IE10 is not tested though */
                a, input, button {
                    -ms-touch-action: none !important;
                }

                .container {
                    margin-top: 20px;
                }

                #emulator > div {
                    text-align: center;
                }

                #screen {
                    height: 500px;
                    /*width: 500px;*/
                    /*margin: auto;*/
                    /*border: 1px solid #000;*/
                }

                #screen canvas {
                    /*background: #000;*/
                    margin: auto;
                    display: block;
                    image-rendering: -moz-crisp-edges; /* FF 6+ */
                    image-rendering: -webkit-optimize-contrast; /* Webkit */
                    image-rendering: pixelated; /* Blink */
                    -ms-interpolation-mode: nearest-neighbor; /* IE 7+ */
                }

                #screen:-webkit-full-screen {
                    height: 100%;
                    width: 100%;
                }

                .gamepad {
                    height: 80px;
                    position: relative;
                }

                div .direction > div,
                div .buttons > div {
                    width: 40px;
                    height: 40px;
                    float: left;
                    display: inline;
                }

                div .direction {
                    background: lightgrey;
                    width: 80px;
                    height: 80px;
                    border-radius: 40px;
                    -webkit-transform: rotate(45deg);
                    transform: rotate(45deg);
                }

                .up, .down {
                    background: darkgrey;
                }

                .up {
                    border-radius: 40px 0 0 0;
                }

                .down {
                    border-radius: 0 0 40px 0;
                }

                div .buttons {
                    position: absolute;
                    right: 0;
                    top: 20px;
                }

                div .buttons .start {
                    background: darkblue;
                    border-radius: 0 40px 40px 0;
                    width: 25px;
                }

                .fire1, .fire2 {
                    background: black;
                    margin-left: 10px;
                    border-radius: 40px;
                }

                #romSelector {
                    clear: both;
                    margin-top: 80px;
                }

                select, .btn {
                    margin: 5px 5px 0 0;
                }

                footer {
                    padding-top: 10px;
                }

                @media (max-width: 768px) {
                    .container {
                        margin-top: 0;
                    }
                }

                @media (orientation: portrait) {
                    #screen canvas {
                        height: auto;
                        width: 100%;
                    }
                }

                @media (orientation: landscape) {
                    #screen canvas {
                        height: 100%;
                        width: auto;
                    }
                }

                /* Debugger */
                div.row-fluid .span8 {
                    position: relative;
                }

                #dissambler {
                    font-family: monospace;
                    font-size: small;
                    color: #aaa;
                    right: 0;
                    position: absolute;
                    top: 0;
                }

                .current {
                    color: #333;
                }

                .current code {
                    background: pink;
                    border-color: #DD1144;
                }

                code {
                    padding: 0 2px;
                }

                .bouton-image:before {
                    content: "";
                    width: 60px;
                    height: 50px;
                    display: inline-block;
                    margin-right: 5px;
                    vertical-align: text-top;
                    background-color: transparent;
                    background-position: center center;
                    background-repeat: no-repeat;
                }

                .monBouton:before {
                    background-image: url(/images/freebird.png);
                    background-size: 60px 50px;
                }
            </style>
        </head>

        <div class="container">
            <div class="content">
                <div class="row-fluid">
                    <div class="span8">
                        <div id="emulator">

                    </div>

                </div>
            </div>
        </div>
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
            $(function() {
                var sms = new JSSMS({
                    'ui': $('#emulator').JSSMSUI({'homebrew': '/apps/emulators/jssms/rom/<?= $rom ?>.sms'})
                });
            })
        </script>
        <script src="/apps/emulators/jssms/src/main.js"></script>
    </div>
<?php $this->stop() ?>