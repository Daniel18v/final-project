<?php

defined("BASEPATH") or exit("No direct script access allowed");

class Emulators_controller extends MY_Controller {

    public function __construct() {
        parent::__construct();
        $this->load->model('emulators_model', 'mdl');
    }

    public function emulator($type = false, $game = false) {
        $game = str_replace("-", " ", $game);
        if ($game) {
            $query = $this->mdl->get_rom($type, $game);
            if (empty($query)) {
                show_404();
            } else {
                $romName = $query[0]->title;
                echo $this->templates->render('emulator::js'.$type, ['sess' => $this, 'rom' => $romName]);
            }
        } else {
            $roms = $this->mdl->get_roms($type);
            echo $this->templates->render('main::platform', ['sess' => $this, 'roms' => $roms]);
        }
    }
}