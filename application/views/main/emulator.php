<?php $this->layout("templates::master") ?>

<?php $this->start('header') ?>
<?php $this->insert("structure::header") ?>
<?php $this->stop() ?>

<?php $this->start('nav') ?>
<?php $this->insert("structure::nav") ?>
<?php $this->stop() ?>

<?php $this->start('section') ?>
<div class="container-fluid mycontent">
    <?php $this->insert("emulator::jsSMS") ?>
</div>
<?php $this->stop() ?>
