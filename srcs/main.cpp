/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmunoz-g <cmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 10:38:08 by juramos           #+#    #+#             */
/*   Updated: 2024/12/13 16:38:41 by cmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IRC.hpp"

// to do carlos:
/*
Message::getParsedCommand()
Gestion switch case 
gestion CAP 

*/

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Uso: " << argv[0] << " <puerto> <contraseña>" << std::endl;
        return 1;
    }
    try {
        Server server(std::atoi(argv[1]), argv[2]);
        server.start();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
