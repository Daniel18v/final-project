<?php defined("BASEPATH") or exit("No direct script access allowed"); ?>

<?php $this->layout("templates::master") ?>

<?php $this->start('header') ?>
<?php $this->insert("structure::header", ['sess' => $sess]) ?>
<?php $this->stop() ?>

<?php $this->start('nav') ?>
<?php $this->insert("structure::nav") ?>
<?php $this->stop() ?>

<?php $this->start('section') ?>
<?php $games = explode(',', $sess->session->userdata('games')) ?>
<div class="row">
    <?php foreach ($roms as $rom => $values) : ?>
        <div class="col-lg-3">
            <div class="panel panel-info">

                <div class="panel-heading">
                    <h3 class="panel-title"><?= substr($values['title'], 0, 50) ?></h3>
                </div>
                <div class="panel-body">
                    <div class="row text-center">
                        <div class="col-lg-12">
                            <div class="thumbnail">
                                <img src="<?=  explode(',',$values['image'])[0] ?>" alt="<?= $values['title'] ?>" class="img-responsive height-250 width-250">
                                <div class="caption">
                                    <h3 class="min-height-80"><?= substr($values['title'], 0, 50) ?></h3>
                                    <p class="min-height-120"><?= substr($values['description'], 0, 160) ?></p>

                                    <?php if(in_array($values['id'], $games)): ?>
                                        <p><a href="/juegos/emulador/<?= $values['type']."/".$values['slug'] ?>" class="btn btn-success"
                                              role="button">Jugar</a>
                                        </p>
                                        <span class="min-height-20">¡Ya tienes este juego!</span>
                                    <?php else: ?>
                                        <p><a class="btn btn-primary buy-game" price="<?= $values['bird_coins'] ?>" val="<?= $values['id'] ?>"
                                              role="button">Comprar</a>
                                        </p>
                                        <span class="min-height-20">BirdCoins: <?= $values['bird_coins'] ?></span> <span class="glyphicon glyphicon-piggy-bank"></span>
                                    <?php endif ?>
                                </div>
                            </div>

                        </div>
                    </div>
                </div>
            </div>
        </div>
    <?php endforeach ?>
    <div class="modal in hidden" id="confirm-buy" style="display: block;">
        <div class="modal-dialog">
            <div class="modal-content">
                <div class="modal-header">
                    <h4 class="modal-title">¿Estás seguro de que deseas comprar esta ROM?</h4>
                </div>
                <div class="modal-body">
                    <p id="menssage-buy"></p>
                    <div class="row">
                        <div class="col-12-xs text-center">
                            <button id="btn-buy" class="btn btn-success btn-md">Yes</button>
                            <button id="btn-cancel" class="btn btn-danger btn-md">No</button>
                        </div>
                    </div>
                </div>

            </div>
        </div>
    </div>
    <div class="modal in hidden" id="alert-buy" style="display: block;">
        <div class="modal-dialog">
            <div class="modal-content">
                <div class="modal-header">
                    <h4 class="modal-title">¡No tienes dinero suficiente!</h4>
                </div>
                <div class="modal-body">
                    <p id="menssage-alert"></p>
                    <div class="row">
                        <div class="col-12-xs text-center">
                            <button id="btn-ok" class="btn btn-danger btn-md">Ok</button>
                        </div>
                    </div>
                </div>

            </div>
        </div>
    </div>
</div>
<script>
    $(document).ready(function () {
        $('.buy-game').click(function () {
            var id = $(this).attr('val');
            var price = $(this).attr('price');
            $('#confirm-buy').removeClass('hidden');
            $('#menssage-buy').html("Se van a descontar <span>" + price + "</span><span class='glyphicon glyphicon-piggy-bank'></span> BirdCoins" );
            $('#btn-cancel').click(function() {
                $('#confirm-buy').addClass('hidden');
            });
            $('#btn-buy').click(function () {
                $.ajax({
                    url : '/buy_game',
                    data : { id : id },
                    type : 'POST',
                    success : function(json) {
                        json = JSON.parse(json);
                        if(json.status == 200) {
                            $('#coins a span').html(json.coins);
                            window.location.reload();
                        } else {
                            showAlert(json.coins)
                        }
                    }
                })
            })
        })
        function showAlert(coins) {
            $('#alert-buy').removeClass('hidden');
            $('#menssage-alert').html("Faltan " + coins + "</span><span class='glyphicon glyphicon-piggy-bank'></span> BirdCoins, para poder comprar esta rom.")
            $('#btn-ok').click(function () {
                $('#alert-buy').addClass('hidden');
                $('#confirm-buy').addClass('hidden');
            })
        }

    })
</script>
<?php $this->stop() ?>
