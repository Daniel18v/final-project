<?php $this->layout("templates::master") ?>

<?php $this->start('header') ?>
<?php $this->insert("structure::header") ?>
<?php $this->stop() ?>

<?php $this->start('nav') ?>
<?php $this->insert("structure::nav") ?>
<?php $this->stop() ?>

<?php $this->start('section') ?>
<div class="container-fluid mycontent">

<head>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8">
    <title>JSNES: A JavaScript NES emulator</title>
    <style>
        .bouton-image:before {
            content: "";
            width: 60px;
            height: 50px;
            display: inline-block;
            margin-right: 5px;
            vertical-align: text-top;
            background-color: transparent;
            background-position : center center;
            background-repeat:no-repeat;
        }

        .monBouton:before{
            background-image : url(/images/freebird.png);
            background-size: 60px 50px;
        }
    </style>
</head>

<body>
<h1>JSNES</h1>

<div id="emulator"></div>

<h2>Controls</h2>
<table id="controls">
    <tr>
        <th>Button</th>
        <th>Player 1</th>
        <th>Player 2</th>
    </tr>
    <tr>
        <td>Left</td>
        <td>Left</td>
        <td>Num-4</td>
    <tr>
        <td>Right</td>
        <td>Right</td>
        <td>Num-6</td>
    </tr>
    <tr>
        <td>Up</td>
        <td>Up</td>
        <td>Num-8</td>
    </tr>
    <tr>
        <td>Down</td>
        <td>Down</td>
        <td>Num-2</td>
    </tr>
    <tr>
        <td>A</td>
        <td>X</td>
        <td>Num-7</td>
    </tr>
    <tr>
        <td>B</td>
        <td>Z/Y</td>
        <td>Num-9</td>
    </tr>
    <tr>
        <td>Start</td>
        <td>Enter</td>
        <td>Num-1</td>
    </tr>
    <tr>
        <td>Select</td>
        <td>Ctrl</td>
        <td>Num-3</td>
    </tr>
</table>

<script src="/apps/jsnes/lib/jquery-1.4.2.min.js" type="text/javascript" charset="utf-8"></script>
<script src="/apps/jsnes/lib/dynamicaudio-min.js" type="text/javascript" charset="utf-8"></script>
<script src="/apps/jsnes/source/nes.js" type="text/javascript" charset="utf-8"></script>
<script src="/apps/jsnes/source/utils.js" type="text/javascript" charset="utf-8"></script>
<script src="/apps/jsnes/source/cpu.js" type="text/javascript" charset="utf-8"></script>
<script src="/apps/jsnes/source/keyboard.js" type="text/javascript" charset="utf-8"></script>
<script src="/apps/jsnes/source/mappers.js" type="text/javascript" charset="utf-8"></script>
<script src="/apps/jsnes/source/papu.js" type="text/javascript" charset="utf-8"></script>
<script src="/apps/jsnes/source/ppu.js" type="text/javascript" charset="utf-8"></script>
<script src="/apps/jsnes/source/rom.js" type="text/javascript" charset="utf-8"></script>
<script src="/apps/jsnes/source/ui.js" type="text/javascript" charset="utf-8"></script>
<script type="text/javascript" charset="utf-8">
    var nes;
    $(function() {
        nes = new JSNES({
            'ui': $('#emulator').JSNESUI({
                "Homebrew": [
                    ['Concentration Room', '/apps/jsnes/roms/croom/croom.nes']
                ]
            })
        });
    });
</script>
<!--[if IE]>
<script type="text/vbscript" src="source/jsnes-ie-hacks.vbscript"></script>
<![endif]-->

</body>
</html>
</div>
<?php $this->stop() ?>