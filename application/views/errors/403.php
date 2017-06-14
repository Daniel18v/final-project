<?php defined("BASEPATH") or exit("No direct script access allowed"); ?>

<?php $this->layout("templates::master") ?>

<?php $this->start('header') ?>
<?php $this->insert("structure::header", ['sess' => $sess]) ?>
<?php $this->stop() ?>

<?php $this->start('nav') ?>
<?php $this->insert("structure::nav") ?>
<?php $this->stop() ?>

<?php $this->start('section') ?>
<h1 class="h1">No tienes permiso para acceder a esta página, por favor. Puede que no estés logueado.<br/>
    Si el problema persiste, contacte con el administrador.</h1>
<?php $this->stop() ?>
