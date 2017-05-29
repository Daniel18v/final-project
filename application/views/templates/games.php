<?php defined("BASEPATH") or exit("No direct script access allowed"); ?>

<!DOCTYPE html>
<html lang="es">

<head>
    <?php $this->insert("structure::head") ?>
</head>

<header class="header">
    <?= (!isset($_SESSION['user'])) ? $this->section("header") : "" ?>
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

<div>
    <?= $this->insert("structure::models") ?>
</div>

<?= $this->section("scripts") ?>


</body>
</html>