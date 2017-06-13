<?php defined("BASEPATH") or exit("No direct script access allowed"); ?>

<!DOCTYPE html>
<html lang="es">

<head>
    <?php $this->insert("structure::head") ?>
    <link rel="stylesheet" href="/css/emulator.css">
    <link rel="stylesheet" href="http://www.giuseppegerbino.com/css/glyphs.css" type="text/css" media="screen"/>
</head>

<header class="header">

</header>

<nav class="navbar navbar-inverse">
    <?= $this->section("nav") ?>
</nav>

<body class="body">

<div class="container-fluid wrapper">
    <?= $this->section("section") ?>
</div>
<div class="push"></div>

<div class="top-footer">
    <?php $this->insert("structure::footer") ?>
</div>

<footer class="footer">
    <?php $this->insert("structure::bottom-footer") ?>
</footer>

<?= $this->section("scripts") ?>

</body>
</html>