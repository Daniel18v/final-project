onload = (d = document).onkeyup = function () {
    (D = d.getElementById("rs-iframe").contentWindow.document).write(html[v = "value"] + "<script>" + js[v] + "<\/script><style>" + css[v]);
    D.close()
}

$(document).ready(function () {
    /*upload image*/
    function readURL(input) {
        if (input.files && input.files[0]) {
            var reader = new FileReader();

            reader.onload = function (e) {
                $('.img-up').attr('src', e.target.result);
                croops();
            }

            reader.readAsDataURL(input.files[0]);
        }
    }

    $("#imgInp").change(function () {
        readURL(this);
        croops
    });
    /*insert game*/
    $('button').click(function () {
        var id = $(this).attr('name');
        $('textarea').addClass('hidden');
        $('#' + id).removeClass('hidden')
    })

})


//CROPS
function croops() {
    jQuery(function ($) {

    // Create variables (in this scope) to hold the API and image size

    var jcrop_api,
        boundx = 100,
        boundy = 100,

        // Grab some information about the preview pane
        $preview = $('#preview-pane'),
        $pcnt = $('#preview-pane .preview-container'),
        $pimg = $('#preview-pane .preview-container img'),

        xsize = $pcnt.width(),
        ysize = $pcnt.height();

    $('#target').Jcrop({
        onChange: updatePreview,
        onSelect: updatePreview,
        aspectRatio: 1,
        minSize: [365, 365],
        maxSize: [365, 365]
    }, function () {
        // Use the API to get the real image size
        var bounds = this.getBounds();
        boundx = bounds[0];
        boundy = bounds[1];
        // Store the API in the jcrop_api variable
        jcrop_api = this;

        // Move the preview into the jcrop container for css positioning
        //$preview.appendTo(jcrop_api.ui.holder);
    });

    function updatePreview(c) {
        if (parseInt(c.w) > 0) {
            var rx = xsize / c.w;
            var ry = ysize / c.h;

            $pimg.css({
                width: Math.round(rx * boundx) + 'px',
                height: Math.round(ry * boundy) + 'px',
                marginLeft: '-' + Math.round(rx * c.x) + 'px',
                marginTop: '-' + Math.round(ry * c.y) + 'px'
            });
        }
    };

});

}
