-- phpMyAdmin SQL Dump
-- version 4.5.1
-- http://www.phpmyadmin.net
--
-- Servidor: 127.0.0.1
-- Tiempo de generación: 22-03-2017 a las 00:40:56
-- Versión del servidor: 10.1.16-MariaDB
-- Versión de PHP: 5.6.24

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Base de datos: `freebird`
--

-- --------------------------------------------------------

--
-- Estructura de tabla para la tabla `roms`
--

CREATE TABLE `roms` (
  `TITLE` varchar(1000) NOT NULL,
  `DESCRIPTION` varchar(10000) NOT NULL,
  `TYPE` varchar(1000) NOT NULL,
  `ROM` varchar(1000) NOT NULL,
  `IMAGES` varchar(10000) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Volcado de datos para la tabla `roms`
--

INSERT INTO `roms` (`TITLE`, `DESCRIPTION`, `TYPE`, `ROM`, `IMAGES`) VALUES
('Super Nintendo Entertainment System (SNES) ROMs', 'ASDFADSFASD', 'SNES', 'ruta-rom/', 'imagen.jpg');

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
