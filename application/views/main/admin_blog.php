<?php defined("BASEPATH") or exit("No direct script access allowed"); ?>

<?php $this->layout("templates::master") ?>
<?php $this->start('header') ?>
<?php $this->insert("structure::header", ['sess' => $sess]) ?>
<?php $this->stop() ?>

<?php $this->start('nav') ?>
<?php $this->insert("structure::nav") ?>
<?php $this->stop() ?>

<?php $this->start('section') ?>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<link rel="apple-touch-icon" href="//mindmup.s3.amazonaws.com/lib/img/apple-touch-icon.png" />
<script src="/js/jquery-3.2.1.min.js"></script>
<script src="/js/bootstrap.min.js"></script>
<!--<link href="/css/bootsrap-combined.no-icons.min.css" rel="stylesheet">-->
<link href="http://netdna.bootstrapcdn.com/twitter-bootstrap/2.3.1/css/bootstrap-responsive.min.css" rel="stylesheet">
<link href="http://netdna.bootstrapcdn.com/font-awesome/3.0.2/css/font-awesome.css" rel="stylesheet">
<script src="/js/jquery-3.2.1.min.js"></script>
<script src="/js/jquery.hotkeys.js"></script>
<script src="/js/bootstrap.min.js"></script>
<script src="/js/google-code-prettify/prettify.js"></script>
<script src="/js/bootstrap-wysiwyg.js"></script>
<script src="/js/events.js"></script>
<link rel="stylesheet" href="/css/custom.css" type="text/css">
<link rel="stylesheet" href="/css/bootstrap.min.css">
<div class="container">
    <h3>Vista de la publicación en el blog</h3>
    <div class="row">
        <div class="col-sm-8 col-md-6 col-lg-5 mt-6">

            <div class="pull-left media">
                <a class="pull-left" href="#">
                    <img class="img-responsive" src="https://cdn1.iconfinder.com/data/icons/toolbar-signs/512/person-512.png" alt="...">
                </a>
            </div>
            <div class="media-body">

                <div class="pull-right"><br>
                    <b><a href="https://www.facebook.com/DLZYagiz">Nombre del usuario</a></b>
                </div><br><br><br>
                <h4 id="title-prev" class="media-heading text-center">Title publicatión</h4>
                <p id="description-prev">Lorem ipsum dolor sit amet, consectetur adipisicing elit. Architecto, consectetur cupiditate ducimus hic iste maiores, natus nisi nobis numquam omnis pariatur quas qui tempore veniam voluptatibus? Asperiores id nostrum veritatis?</p>
                <img id="img-prev" name="image-blog" src="/images/anonymous.jpg" class="img-responsive">
                <ul class="list-inline list-unstyled">
                    <li><span><i class="glyphicon glyphicon-calendar"></i> 2 days, 8 hours </span></li>
                    <li>|</li>
                    <span><i class="glyphicon glyphicon-comment"></i> 2 comments</span>
                    <li>|</li>
                </ul>
            </div>
        </div>
        <div class="col-sm-8 col-md-6 col-lg-5 mt-6">
            <div class="col-md-6">
                <div class="form-group">
                    <label for="">Título(min:3, max: 20)</label>
                    <input id="title-pub" pattern=".{0}|.{3,50}" maxlength="50" required type="text" class="form-control" name="title-blog" autocomplete="off" placeholder="Título...">
                </div>
            </div>
            <div class="col-md-12">
                <div class="form-group">
                    <label for="">Descripción(min:5, max:300)</label>
                    <textarea class="form-control textarea" maxlength="300" minlength="5" required rows="3" name="description-blog" id="description-pub" placeholder="Descripción..."></textarea>
                </div>
            </div>
            <div class="col-md-6">
                <div class="form-group">
                    <label>Ruta de la imagen: </label>
                    <input id="image-pub" type="text"><br>
                </div>
            </div>
        </div>
    </div>
    <div class="row">
        <div class="hero-unit">
            <h3>Publicación</h3>
            <div id="alerts"></div>
            <div class="btn-toolbar" data-role="editor-toolbar" data-target="#editor">
                <div class="btn-group">
                    <a class="btn dropdown-toggle" data-toggle="dropdown" title="Font"><i class="icon-font"></i><b class="caret"></b></a>
                    <ul class="dropdown-menu">
                    </ul>
                </div>
                <div class="btn-group">
                    <a class="btn dropdown-toggle" data-toggle="dropdown" title="Font Size"><i class="icon-text-height"></i>&nbsp;<b class="caret"></b></a>
                    <ul class="dropdown-menu">
                        <li>
                            <a data-edit="fontSize 5">
                                <font size="5">Huge</font>
                            </a>
                        </li>
                        <li>
                            <a data-edit="fontSize 3">
                                <font size="3">Normal</font>
                            </a>
                        </li>
                        <li>
                            <a data-edit="fontSize 1">
                                <font size="1">Small</font>
                            </a>
                        </li>
                    </ul>
                </div>
                <div class="btn-group">
                    <a class="btn" data-edit="bold" title="Bold (Ctrl/Cmd+B)"><i class="icon-bold"></i></a>
                    <a class="btn" data-edit="italic" title="Italic (Ctrl/Cmd+I)"><i class="icon-italic"></i></a>
                    <a class="btn" data-edit="strikethrough" title="Strikethrough"><i class="icon-strikethrough"></i></a>
                    <a class="btn" data-edit="underline" title="Underline (Ctrl/Cmd+U)"><i class="icon-underline"></i></a>
                </div>
                <div class="btn-group">
                    <a class="btn" data-edit="insertunorderedlist" title="Bullet list"><i class="icon-list-ul"></i></a>
                    <a class="btn" data-edit="insertorderedlist" title="Number list"><i class="icon-list-ol"></i></a>
                    <a class="btn" data-edit="outdent" title="Reduce indent (Shift+Tab)"><i class="icon-indent-left"></i></a>
                    <a class="btn" data-edit="indent" title="Indent (Tab)"><i class="icon-indent-right"></i></a>
                </div>
                <div class="btn-group">
                    <a class="btn" data-edit="justifyleft" title="Align Left (Ctrl/Cmd+L)"><i class="icon-align-left"></i></a>
                    <a class="btn" data-edit="justifycenter" title="Center (Ctrl/Cmd+E)"><i class="icon-align-center"></i></a>
                    <a class="btn" data-edit="justifyright" title="Align Right (Ctrl/Cmd+R)"><i class="icon-align-right"></i></a>
                    <a class="btn" data-edit="justifyfull" title="Justify (Ctrl/Cmd+J)"><i class="icon-align-justify"></i></a>
                </div>
                <div class="btn-group">
                    <a class="btn dropdown-toggle" data-toggle="dropdown" title="Hyperlink"><i class="icon-link"></i></a>
                    <div class="dropdown-menu input-append">
                        <input class="span2" placeholder="URL" type="text" data-edit="createLink" />
                        <button class="btn" type="button">Add</button>
                    </div>
                    <a class="btn" data-edit="unlink" title="Remove Hyperlink"><i class="icon-cut"></i></a>

                </div>

                <div class="btn-group">
                    <a class="btn" title="Insert picture (or just drag & drop)" id="pictureBtn"><i class="icon-picture"></i></a>
                    <input type="file" data-role="magic-overlay" data-target="#pictureBtn" data-edit="insertImage" />
                </div>
                <div class="btn-group">
                    <a class="btn" data-edit="undo" title="Undo (Ctrl/Cmd+Z)"><i class="icon-undo"></i></a>
                    <a class="btn" data-edit="redo" title="Redo (Ctrl/Cmd+Y)"><i class="icon-repeat"></i></a>
                </div>
                <input type="text" data-edit="inserttext" id="voiceBtn" x-webkit-speech="">
            </div>
            <div id="editor">
            </div>
            <button class="btn btn-success" id="btn-post">Publicar artículo</button>
            <button id="preview" class="btn btn-success">Vista previa</button><br/><br/>
            <div id="rs"></div>
        </div>
    </div>
</div>
</body>
<script>
    $(document).ready(function () {
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
        function createSlug(title) {
            var slug = title.trim()
                .replace(/(\.|\,)+/, "")
                .replace(/\s/g, "-")
                .replace(/(á)+/g, 'a')
                .replace(/(é)+/g, 'e')
                .replace(/(í)+/g, 'i')
                .replace(/(ó)+/g, 'o')
                .replace(/(ú)+/g, 'u')
                .toLowerCase();

            return /blog/ + slug;
        }
        $('#btn-post').click(function () {
            var img = $('[name=image-blog]').attr('src');
            var title = $('input[name=title-blog]').val();
            var description = $('textarea[name=description-blog]').val();
            var publishing = $('#editor').html();
            if (img != '' && title != '' && description != '' && publishing != '') {
                $.ajax({
                    url: '/admin/blog/insert',
                    data: {
                        title: title,
                        slug: createSlug(title),
                        description: description,
                        image: img,
                        publishing: publishing,
                        timestamp: getTimestamp()
                    },
                    type: 'POST',
                    dataType: 'json',
                    success: function (status) {
                        if(status == 200) {
                            alert('Publicado')
                            window.location.reload();
                        }
                    }

                })
            } else {
                alert('¡¡Rellene todos los campos!!')
            }
        })
    })
</script>
</html>
<?php $this->stop() ?>

