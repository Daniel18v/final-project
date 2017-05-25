<?php defined("BASEPATH") or exit("No direct script access allowed"); ?>

<div class="container-fluid">
    <div class="row">
        <div class="col-6-md col-xs-4">
            <a href="<?= site_url() ?>">
                <img src="/images/header.png" class="img-responsive pull-left" alt="Cabecera"/>

            </a>
        </div>

        <?php if (!$sess->session->userdata('user')) :?>
        <div class="col-6-md col-xs-8">
            <p>
                <button type="button" class="btn btn-sm btn-info pull-right login"><span class="glyphicon glyphicon-log-in"></span>  Iniciar sesión</button>

                <button type="button" class="margin-right-5 btn btn-sm btn-danger pull-right signup"><span class="glyphicon glyphicon-new-window"></span> Registrarse</button>
            </p>
        </div>
        <?php else: ?>
                <img src="<?= base_url($sess->session->userdata('user')) ?>" class="img-responsive" width="250" height="250"/>
        <?php endif ?>
    </div>
</div>
