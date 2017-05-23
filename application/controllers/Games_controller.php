<?php

defined("BASEPATH") or exit("No direct script access allowed");

class Games_controller extends MY_Controller {

    public function __construct() {
        parent::__construct();
        $this->load->model('main_model');
    }
    public function emulator($type = false, $name_url = false) {
        $rom = false;
        switch (strtolower($type)) {
            case "nes":
                if($name_url) {
                    $rom = $this->main_model->get_rom($name_url, $type);
                }
                echo $this->templates->render('emulator::jsNES', ['session' => $this, 'rom' => $rom]);
                break;
            case "sms":
                echo $this->templates->render('emulator::jsSMS', ['session' => $this, 'rom' => $rom]);
                break;
            default:
                show_404();
        }

        //$data = $this->Main_model->get_id_roms($roms);
        //echo $this->templates->render('emulator::jsNES', $data);
    }

    public function games($p = false) {
        switch (strtolower($p)) {
            case "flappybird" :
                echo $this->templates->render("games::flappy",  ['session' => $this]);
                break;
            case "galaga" :
                echo $this->templates->render("games::galaga",  ['session' => $this]);
                break;
            default:
                echo $this->templates->render('main::games', ['session' => $this]);
                break;
        }
    }

}