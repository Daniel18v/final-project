$(document).ready(function () {
    /* MODELS */

    $(".signup").click(function () {
        $("#formSignup").modal();
    });

    $(".login").click(function () {
        $("#formLogin").modal();
    });
    $(".nes").click(function () {
        $(location).attr('href', '/juegos/emulador/nes');
    });

    $(".sms").click(function () {
        $(location).attr('href', '/juegos/emulador/sms');
    });

    /* GALAGA */
    if ($(location).attr('href') === document.location.origin + '/juegos/galaga') {
        $("#galaga").ready(function () {
            main();
        });
    }
    $('.day-night').click(function () {
        $('body').toggleClass('night');
    });

});
