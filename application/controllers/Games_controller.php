<?

defined("BASEPATH") or exit("No direct script access allowed");

class Games_controller extends MY_Controller {
    public function emulator($p = false, $roms = false) {
        switch (strtolower($p)) {
            case "nes":
                echo $this->templates->render('emulator::jsNES');
                break;
            case "sms":
                echo $this->templates->render('emulator::jsSMS');
                break;
            default:
                show_404();
        }

        //$data = $this->Main_model->get_id_roms($roms);
        //echo $this->templates->render('emulator::jsNES', $data);
    }

    public function games($p = false) {
        switch (strtolower($p)) {
            case "galaga" :
                echo $this->templates->render("games::galaga");
                break;
            default:
                echo $this->templates->render('main::games');
                break;
        }
    }

}