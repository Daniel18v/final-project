$(document).ready(function() {
    /* MODELS */
    $("#signin").click(function(){
        $("#formSignin").modal();
    });
    $("#login").click(function() {
       $("#formLogin").modal();
    });

    /* GALAGA */
    $("#galaga").ready(function() {
        main();
    });
});