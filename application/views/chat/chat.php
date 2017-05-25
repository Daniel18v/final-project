<?php defined("BASEPATH") or exit("No direct script access allowed"); ?>
<div class="embed-responsive embed-responsive-4by3">
    <?php if($sess->session->userdata('user')): ?>
        <iframe width="560" height="900" src="http://freebird.ddns.net:3030/?user=<?= $sess->session->userdata('user') ?>" frameborder="0" allowfullscreen></iframe>
    <?php else: ?>
        <label>Inicia sesión para poder hablar en el chat</label>
        <button type="button" class="btn btn-sm btn-info pull-right login"><span class="glyphicon glyphicon-log-in"></span>  Iniciar sesión</button>
    <?php endif ?>
</div>