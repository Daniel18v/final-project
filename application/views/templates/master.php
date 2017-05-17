<!DOCTYPE html>
<html lang="es">

<head>
    <?php $this->insert("structure::head") ?>
</head>

<header class="header">
    <?= $this->section("header") ?>
</header>

<nav class="navbar navbar-inverse">
    <?= $this->section("nav") ?>
</nav>

<body class="body">

<?= $this->section("section") ?>

<footer class="footer">
    <?php $this->insert("structure::footer") ?>
</footer>
</body>
</html>