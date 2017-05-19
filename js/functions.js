/*
$(document).ready(function () {
    var singin = $("#id01");
    var login = $("#id02");
    $("#singin").on("click", function () {
        singin.css("display", 'block');
    });

    $("#login").on("click", function () {
        login.css("display", 'block');
    });

    $(".close").on("click", function () {
        hideThis(singin);
        hideThis(login);
    });
    $(".cancel").on("click", function () {
        hideThis(singin);
        hideThis(login);
    });


    function hideThis(e) {
        e.css("display", 'none');
    }
});
*/


$(document).ready(function() {
    $("#signin").click(function(){
        $("#formSignin").modal();
    });
    $("#login").click(function() {
       $("#formLogin").modal();
    });
});