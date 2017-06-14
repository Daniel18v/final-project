<?php defined("BASEPATH") or exit("No direct script access allowed"); ?>

<?php $this->layout("templates::master") ?>

<?php $this->start('header') ?>
<?php $this->insert("structure::header", ['sess' => $sess]) ?>
<?php $this->stop() ?>

<?php $this->start('nav') ?>
<?php $this->insert("structure::nav") ?>
<?php $this->stop() ?>

<?php $this->start('section') ?>
        <div class="container">
            <div class="row">
                <div class="panel panel-info posted-blog">
                    <div class="pull-left media">

                    </div>
                    <div class="media-body">
                        <h1 id="title-prev" class="media-heading text-center"><?= $blog->title ?></h1>
                        <img id="img-prev" src="<?= $blog->image ?>" class="img-responsive">
                        <p id="description-prev"><?= $blog->publishing ?></p>

                        <ul class="list-inline list-unstyled">
                            <li><span><i class="glyphicon glyphicon-calendar"></i> <?= $blog->timestamp ?> </span></li>
                            <li>|</li>
                        </ul>
                    </div>
                </div>
            </div>
            <!-- Comments -->
            <?php if ($sess->session->userdata('user')) : ?>
            <div class="row">
                <div class="col-md-8">
                    <h2 class="page-header">Comentarios</h2>
                    <section class="comment-list">
                        <!-- Commentary -->
                        <?php foreach ($comments as $post => $values) : ?>
                        <article class="row">
                            <div class="col-md-2 col-sm-2 hidden-xs">
                                <figure class="thumbnail">
                                    <img class="img-responsive" src="<?= $values->user_avatar ?>" />
                                    <figcaption class="text-center"><a href="<?= "/perfil/" . $values->user ?>"> <?= $values->user ?></a></figcaption>
                                </figure>
                            </div>
                            <div class="col-md-10 col-sm-10">
                                <div class="panel panel-default arrow left">
                                    <div class="panel-body">
                                        <header class="text-left">
                                            <div class="comment-user"><i class="fa fa-user"></i><a href="<?= "/perfil/" . $values->user ?>"> <?= $values->user ?></a></div>
                                            <time class="comment-date" datetime="<?= $values->timestamp ?>"><i class="fa fa-clock-o"></i> <?= $values->timestamp ?></time>
                                        </header>
                                        <div class="comment-post">
                                            <p>
                                                <?= $values->commentary ?>
                                            </p>
                                        </div>
                                    </div>
                                </div>
                            </div>
                        </article>
                        <?php endforeach; ?>
                    </section>
                </div>
            </div>
            <div class="row">
                <div class="col-md-10 clear"><hr><h2>Deja tu comentario</h2></div>
                <div class="col-md-10">
                    <div class="container-fluid well span8">
                        <div class="panel panel-default">
                            <div class="panel-body">
                                <textarea class="form-control counted" name="commentary" placeholder="Escribe tu mensaje" rows="5" style="margin-bottom:10px;"></textarea>
                                <button class="btn btn-info" id="send-comment">Comentar</button>
                            </div>
                        </div>
                    </div>
                </div>
            </div>
            <?php endif ?>
        </div>
    </div>
</div>
<script>
    $(document).ready(function () {
        $('textarea').keypress(function (e) {

            if (e.which == 13) {
                commentBlog();
            }

        });
        function addZero(i) {
            if (i < 10) {
                i = "0" + i;
            }
            return i;
        }
        function getTimestamp() {
            var today = new Date();
            var h = addZero(today.getHours());
            var m = addZero(today.getMinutes());
            var s = addZero(today.getSeconds());
            var time = h + ":" + m + ":" + s;
            var dd = today.getDate();
            var mm = today.getMonth()+1; //January is 0!
            var yyyy = today.getFullYear();

            if(dd<10) {
                dd='0'+dd
            }

            if(mm<10) {
                mm='0'+mm
            }
            return  yyyy +'-' + mm+'-'+dd + ' ' + time;
        }
        function commentBlog() {
            if($('textarea[name=commentary]').val().length > 5) {
                $.ajax({
                    url: window.location.href + '/comment',
                    data: {
                        commentary: $('textarea[name=commentary]').val(),
                        timestamp: getTimestamp()
                    },
                    type: 'POST',
                    dataType: 'json',
                    success: function (status) {
                        if(status == 200) {
                            window.location.reload();
                        }
                    }
                })
                $.ajax({
                    url: '/add_coins_comment',
                    data: {},
                    type: 'POST'
                })
            }

        }
        $('#send-comment').click(function () {
            commentBlog()
        })

    })
</script>
<?php $this->stop() ?>
