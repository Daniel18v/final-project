<?php defined("BASEPATH") or exit("No direct script access allowed"); ?>

<?php $this->layout("templates::master") ?>

<?php $this->start('header') ?>
<?php $this->insert("structure::header", ['sess' => $sess]) ?>
<?php $this->stop() ?>

<?php $this->start('nav') ?>
<?php $this->insert("structure::nav") ?>
<?php $this->stop() ?>

<?php $this->start('section') ?>
<h1 class="h1">No se ha encontrado la página solicitada o no existe.</h1>
<?php $this->stop() ?>
