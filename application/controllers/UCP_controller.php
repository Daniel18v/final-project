<?php
defined("BASEPATH") or exit("No direct script access allowed");

class UCP_controller extends MY_Controller {

    /**
     * Inherits of MY_Controller and charges the model for this controller.
     */
    public function __construct() {
        parent::__construct();
        $this->load->model('User_model', 'mod');
    }

    public function profile($user = FALSE) {

        if ($user) {
            $data = $this->mod->select_user($user);
            echo $this->templates->render('main::UCP-extern', ['sess' => $this, 'user' => $data]);
        } else {
            echo $this->templates->render('main::UCP', ['sess' => $this]);

        }

    }

    public function edit() {
        echo $this->templates->render('main::UCP-edit', ['sess' => $this]);
    }

    public function update_profile() {
        $this->mod->update_profile($this->input->post());
        $this->session->set_userdata($this->input->post());
        header('Location: /perfil');
    }

    public function show_games() {
        $games_user = explode(',', $this->mod->get_games($this->session->userdata('user')));
        $where = "";
        for($i = 0; $i < count($games_user); $i++) {
            if($i < count($games_user) -1) {
                $where .= " id = '" .$games_user[$i] . "' OR ";
            } else {
                $where .= " id = '" . $games_user[$i] . "'";
            }

        }
        $roms = $this->mod->get_roms($where);
        echo $this->templates->render('main::UCP-games', ['sess' => $this, 'roms' => $roms]);
    }
}
