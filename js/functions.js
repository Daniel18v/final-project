$(document).ready(function () {
    /* MODELS */

    $(".signup").click(function () {
        $("#formSignup").modal();
    });

    $(".login").click(function () {
        $("#formLogin").modal();
    });
    $(".nes").click(function () {
        $(location).attr('href', 'emulador/nes');
    });

    $(".sms").click(function () {
        $(location).attr('href', 'emulador/sms');
    });

    /* GALAGA */
    if ($(location).attr('href') === 'http://freebird.ddns.net/juegos/galaga') {
        $("#galaga").ready(function () {
            main();
        });
    }
    $('.day-night').click(function () {
        $('body').toggleClass('night');
    });

});
