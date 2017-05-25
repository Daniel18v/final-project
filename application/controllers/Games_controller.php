<?php

defined("BASEPATH") or exit("No direct script access allowed");

class Games_controller extends MY_Controller {

    public function __construct() {
        parent::__construct();
        $this->load->model('main_model');
    }

    public function games($p = false) {
        switch (strtolower($p)) {
            case "flappybird" :
                echo $this->templates->render("games::flappy",  ['sess' => $this]);
                break;
            case "galaga" :
                echo $this->templates->render("games::galaga",  ['sess' => $this]);
                break;
            default:
                echo $this->templates->render('main::games', ['sess' => $this]);
                break;
        }
    }

}