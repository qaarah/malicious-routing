/* Copyright (C) 2015 Cotton Seed
   
   This file is part of arachne-pnr.  Arachne-pnr is free software;
   you can redistribute it and/or modify it under the terms of the GNU
   General Public License version 2 as published by the Free Software
   Foundation.
   
   This program is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program. If not, see <http://www.gnu.org/licenses/>. */

#include "util.hh"
#include "netlist.hh"
#include "line_parser.hh"
#include "bitvector.hh"
#include "casting.hh"

#include <cctype>
#include <cstring>
#include <istream>
#include <fstream>
#include <iostream>
#include <string>

class BlifParser : public LineParser
{
  BitVector stobv(const std::string &s_);
  
public:
  BlifParser(const std::string &f, std::istream &s_)
    : LineParser(f, s_)
  {}
  
  Design *parse();
};

BitVector
BlifParser::stobv(const std::string &s_)
{
  int n = s_.size();
  BitVector bv(n);
  for (int i = 0; i < n; ++i)
    {
      char c = s_[(n - 1) - i];
      if (c == '1')
        bv[i] = true;
      else if (c == '0'
               || c == 'x'
               || c == 'X')

        ;
      else
        fatal("invalid character in integer constant");
    }
  return bv;
}

Design *
BlifParser::parse()
{
  Design *d = new Design;
  d->create_standard_models();
  
  Model *io_model = d->find_model("SB_IO");
  Model *io_od_model = d->find_model("SB_IO_OD");
  Model *io_od_a_model = d->find_model("SB_IO_OD_A");

  Model *top = nullptr;
  
  std::vector<std::pair<Net *, Net *>> unify;
  
  Instance *inst = nullptr;


  std::ostream *null_ostream = nullptr;
  //logs = null_ostream = new std::ostream(new null_streambuf);
  logs = &std::cerr;
  *logs << "TestBlif1...\n";
  ///////Test
  std::string expanded = expand_filename("generated.blif");
  std::ofstream fs(expanded);
  if (fs.fail())
    fatal(fmt("write_txt: failed to open `" << expanded << "': "
              << strerror(errno)));
  fs << "Hello Test Generated blif!" << "\n";

  ////////Test
  for (;;)
    {
      if (eof())
        goto M;

      // parse current line into words
      read_line();
      //////Test
      fs << line  << "\n";
      //////Test
      if (line.empty())
        continue;

      // all directives begin with a dot
      if (line[0] == '.')
        {
        L:
          std::string cmd = words[0];
          if (cmd == ".model")
            {
              if (words.size() != 2)
                fatal(fmt("invalid .model directive: expected exactly 1 argument, got " << words.size()-1));
              if (top)
                fatal("definition of multiple models is not supported");

              top = new Model(d, words[1]);
              d->set_top(top);
            }
          else if (cmd == ".inputs")
            {
              if (!top)
                fatal(".inputs directive outside of model definition");

              for (unsigned i = 1; i < words.size(); i ++)
                {
                  Port *port = top->find_port(words[i]);
                  if (port)
                    {
                      if (port->direction() == Direction::OUT)
                        port->set_direction(Direction::INOUT);
                      *logs << "TestBlif2..." << words[i] << " \n";
                    }
                  else{
                    port = top->add_port(words[i], Direction::IN);
                    *logs << "TestBlif3..." << words[i] << " \n";
                  }
                  Net *net = top->find_or_add_net(words[i]);
                  port->connect(net);
                }
                ////Test
                Port *port = top->find_port("s");
                if (port)
                  {
                    if (port->direction() == Direction::OUT)
                      port->set_direction(Direction::INOUT);
                    *logs << "TestBlif22..." << "s" << " \n";
                  }
                else{
                  port = top->add_port("s", Direction::IN);
                  *logs << "TestBlif33..." << "s" << " \n";
                }
                Net *net = top->find_or_add_net("s");
                port->connect(net);
                ////Test

            }
          else if (cmd == ".outputs")
            {
              if (!top)
                fatal(".outputs directive outside of model definition");

              for (unsigned i = 1; i < words.size(); i ++)
                {
                  Port *port = top->find_port(words[i]);
                  if (port)
                    {
                      if (port->direction() == Direction::IN)
                        port->set_direction(Direction::INOUT);
                    }
                  else
                    port = top->add_port(words[i], Direction::OUT);
                  Net *net = top->find_or_add_net(words[i]);
                  port->connect(net);
                }
            }
          else if (cmd == ".names")
            {
              if (!top)
                fatal(".names directive outside of model definition");

              LexicalPosition names_lp = lp;
              
              Net *names_net = nullptr;
              unsigned n = words.size();

              // output is assigned no value; set to zero
              if (n == 2)
                {
                  names_net = top->find_or_add_net(words[1]);
                  names_net->set_is_constant(true);
                  names_net->set_constant(Value::ZERO);
                }

              // output is assigned input; unify nets
              else if (n == 3)
                {
                  unify.push_back(std::make_pair(top->find_or_add_net(words[1]),
                                                 top->find_or_add_net(words[2])));
                }
              else
                fatal(fmt("invalid .names directive: expected 1 or 2 arguments, got " << n-1));

              // parse PLA-style configuration
              bool saw11 = false;
              for (;;)
                {
                  if (eof())
                    {
                      if (n == 3
                          && !saw11)
                        names_lp.fatal("invalid .names directive: unexpected end of file");
                      goto M;
                    }
                  
                  read_line();
                  
                  if (line.empty())
                    continue;
                  
                  if (line[0] == '.')
                    {
                      if (n == 3
                          && !saw11)
                        names_lp.fatal("invalid .names directive: .names entry expected");
                      goto L;
                    }
                  
                  if (words.size() != n - 1)
                    fatal("invalid .names entry: number of gates does not match specified number of nets");
                  
                  // .names + 1 argument
                  if (n == 2)
                    {
                      const std::string &output = words[0];
                      if (output == "1")
                        names_net->set_constant(Value::ONE);
                      else if (output != "0")
                        fatal("invalid .names entry: gate must be either 1 or 0");
                    }
                  else
                    {
                      assert(n == 3);
                      if (words[0] != "1"
                          || words[1] != "1")
                        fatal("invalid .names entry: both gates must be 1 here");
                      saw11 = true;
                    }
                }
            }
          else if (cmd == ".gate")
            {
              if (!top)
                fatal(".gate directive outside of model definition");

              std::string &n = words[1];
              Model *inst_of = d->find_model(n);

              if (words.size() < 2)
                fatal("invalid .gate directive: missing name");
              {
              *logs << "TestBlif4..." << words[1] << " \n";

              if (!inst_of)
                fatal(fmt("unknown model `" << n << "'"));
              
              inst = top->add_instance(inst_of);
              
              for (unsigned i = 2; i < words.size(); i ++)
                {
                  *logs << "TestBlif4words..." << words[i] << " \n";
                  //////Test
                  if ( words[i]=="Q=d_out[0]"){
                    words[i]="Q=d_out1[0]";
                    *logs << "+TestBlif4words Altered..." << words[i] << " \n";
                  }
                  *logs << "TestBlif4words..." << words[i] << " \n";
                  //////Test
                  if ( words[i]=="Q=d_out[1]"){
                    words[i]="Q=d_out1[1]";
                    *logs << "+TestBlif4words Altered..." << words[i] << " \n";
                  }
                  *logs << "TestBlif4words..." << words[i] << " \n";
                  //////Test
                  if ( words[i]=="Q=d_out[2]"){
                    words[i]="Q=d_out1[2]";
                    *logs << "+TestBlif4words Altered..." << words[i] << " \n";
                  }
                  *logs << "TestBlif4words..." << words[i] << " \n";
                  //////Test
                  if ( words[i]=="Q=d_out[3]"){
                    words[i]="Q=d_out1[3]";
                    *logs << "+TestBlif4words Altered..." << words[i] << " \n";
                  }
                  // *logs << "TestBlif4words..." << words[i] << " \n";
                  // //////Test
                  // if ( words[i]=="Q=d_out[4]"){
                  //   words[i]="Q=d_out1[4]";
                  //   *logs << "+TestBlif4words Altered..." << words[i] << " \n";
                  // }
                  // *logs << "TestBlif4words..." << words[i] << " \n";
                  // //////Test
                  // if ( words[i]=="Q=d_out[5]"){
                  //   words[i]="Q=d_out1[5]";
                  //   *logs << "+TestBlif4words Altered..." << words[i] << " \n";
                  // }
                  // *logs << "TestBlif4words..." << words[i] << " \n";
                  // //////Test
                  // if ( words[i]=="Q=d_out[6]"){
                  //   words[i]="Q=d_out1[6]";
                  //   *logs << "+TestBlif4words Altered..." << words[i] << " \n";
                  // }
                  // *logs << "TestBlif4words..." << words[i] << " \n";
                  // //////Test
                  // if ( words[i]=="Q=d_out[7]"){
                  //   words[i]="Q=d_out1[7]";
                  //   *logs << "+TestBlif4words Altered..." << words[i] << " \n";
                  // }
                  //////Test
                  const std::string &w = words[i];
                  std::size_t p = w.find('=');
                  if (p == std::string::npos)
                    fatal("invalid formal-actual");
                  
                  std::string formal(w, 0, p),
                    actual(w, p+1);
                  
                  if (actual.empty())
                    continue;
                  
                  Port *port = inst->find_port(formal);
                  if (!port)
                    fatal(fmt("unknown formal `" << formal << "'"));
                  
                  Net *net = top->find_or_add_net(actual);
                  port->connect(net);
                }
              }
            }
          else if (cmd == ".attr")
            {
              if (words.size() != 3)
                fatal(fmt("invalid .attr directive: expected exactly 2 arguments, got " << words.size()-1));
              if (!inst)
                fatal("no gate for .attr directive");
              
              if (words[2][0] == '"')
                {
                  *logs << "TestAttr1..." << " words1 " << words[1] << " words2 " << words[2] << " \n";
                  assert(words[2].back() == '"');
                  inst->set_attr(words[1], 
                                 Const(lp, words[2].substr(1, words[2].size() - 2)));
                }
              else
                {
                  *logs << "TestAttr2..." << " words1 " << words[1] << " words2 " << words[2] << " \n";
                  inst->set_attr(words[1],
                                 Const(lp, stobv(words[2])));
                }
            }
          else if (cmd == ".param")
            {
              if (words.size() != 3)
                fatal(fmt("invalid .param directive: expected exactly 2 arguments, got " << words.size()-1));
              if (!inst)
                fatal("no gate for .param directive");
              
              if (words[2][0] == '"')
                {
                  *logs << "TestBlif5..." << words[2][0] << " \n";
                  assert(words[2].back() == '"');
                  inst->set_param(words[1], 
                                  Const(lp, words[2].substr(1, words[2].size() - 2)));
                }
              else
                {
                  *logs << "TestBlif6..." << words[1]  << " " << words[2] << " \n";
                  inst->set_param(words[1],
                                  Const(lp, stobv(words[2])));
                }
                              
            }
          else if (cmd == ".end")
            {
              if (!top)
                fatal(".end directive outside of model definition");

                /////////////////////////////////////Test
              const std::string &n = "SB_LUT4";
              *logs << "+TestBlif Added44..." << "SB_LUT4" << " \n";

              Model *inst_of = d->find_model(n);
              
              inst = top->add_instance(inst_of);
              std::string ss = "\"./usr/local/bin/../share/yosys/ice40/cells_map.v:22.34-23.52\"";
              *logs << "+TestAttr Added11..." << " words1 " << "src" << " words2 " << ss << " \n";
                assert(ss.back() == '"');
                  inst->set_attr("src", 
                                 Const(lp, ss.substr(1, ss.size() - 2)));

                {*logs << "+TestAttr Added22..." << " words1 " << "module_not_derived" << " words2 " << "00000000000000000000000000000001" << " \n";
                  inst->set_attr("module_not_derived",
                                 Const(lp, stobv("00000000000000000000000000000001")));
                }

                {*logs << "+TestBlif Added66..." << "LUT_INIT"  << " " << "1111000011001100" << " \n";
                inst->set_param("LUT_INIT",
                                Const(lp, stobv("1111000011001100")));
                }
              //for (unsigned i = 2; i < words.size(); i ++)
                {
                  *logs << "+TestBlif Added44words..." << "I0=$false" << " \n";
                  const std::string &w = "I0=$false";
                  std::size_t p = w.find('=');
                  if (p == std::string::npos)
                    fatal("invalid formal-actual");
                  
                  std::string formal(w, 0, p),
                    actual(w, p+1);
                  
                  Port *port = inst->find_port(formal);
                  if (!port)
                    fatal(fmt("unknown formal `" << formal << "'"));
                  
                  Net *net = top->find_or_add_net(actual);
                  port->connect(net);
                }
                {
                  *logs << "+TestBlif Added44words..." << "I1=key.r0[0]" << " \n";
                  const std::string &w = "I1=key.r0[0]";
                  std::size_t p = w.find('=');
                  if (p == std::string::npos)
                    fatal("invalid formal-actual");
                  
                  std::string formal(w, 0, p),
                    actual(w, p+1);
                  
                  
                  Port *port = inst->find_port(formal);
                  if (!port)
                    fatal(fmt("unknown formal `" << formal << "'"));
                  
                  Net *net = top->find_or_add_net(actual);
                  port->connect(net);
                }
                {
                  *logs << "+TestBlif Added44words..." << "I2=d_out1[0]" << " \n";
                  const std::string &w = "I2=d_out1[0]";
                  std::size_t p = w.find('=');
                  if (p == std::string::npos)
                    fatal("invalid formal-actual");
                  
                  std::string formal(w, 0, p),
                    actual(w, p+1);
                  
                  
                  Port *port = inst->find_port(formal);
                  if (!port)
                    fatal(fmt("unknown formal `" << formal << "'"));
                  
                  Net *net = top->find_or_add_net(actual);
                  port->connect(net);
                }
                {
                  *logs << "+TestBlif Added44words..." << "I3=s" << " \n";
                  const std::string &w = "I3=s";
                  std::size_t p = w.find('=');
                  if (p == std::string::npos)
                    fatal("invalid formal-actual");
                  
                  std::string formal(w, 0, p),
                    actual(w, p+1);
                  
                  
                  Port *port = inst->find_port(formal);
                  if (!port)
                    fatal(fmt("unknown formal `" << formal << "'"));
                  
                  Net *net = top->find_or_add_net(actual);
                  port->connect(net);
                }
                {
                  *logs << "+TestBlif Added44words..." << "O=d_out[0]" << " \n";
                  const std::string &w = "O=d_out[0]";
                  std::size_t p = w.find('=');
                  if (p == std::string::npos)
                    fatal("invalid formal-actual");
                  
                  std::string formal(w, 0, p),
                    actual(w, p+1);
                  
                  
                  Port *port = inst->find_port(formal);
                  if (!port)
                    fatal(fmt("unknown formal `" << formal << "'"));
                  
                  Net *net = top->find_or_add_net(actual);
                  port->connect(net);
                }
                //Test d_out[1]
              const std::string &n1 = "SB_LUT4";
              *logs << "+TestBlif Added44..." << "SB_LUT4" << " \n";

              Model *inst_of_1 = d->find_model(n1);
              
              inst = top->add_instance(inst_of_1);
              std::string ss1 = "\"./usr/local/bin/../share/yosys/ice40/cells_map.v:22.34-23.52\"";
              *logs << "+TestAttr Added11..." << " words1 " << "src" << " words2 " << ss1 << " \n";
                assert(ss1.back() == '"');
                  inst->set_attr("src", 
                                 Const(lp, ss1.substr(1, ss1.size() - 2)));

                {*logs << "+TestAttr Added22..." << " words1 " << "module_not_derived" << " words2 " << "00000000000000000000000000000001" << " \n";
                  inst->set_attr("module_not_derived",
                                 Const(lp, stobv("00000000000000000000000000000001")));
                }

                {*logs << "+TestBlif Added66..." << "LUT_INIT"  << " " << "1111000011001100" << " \n";
                inst->set_param("LUT_INIT",
                                Const(lp, stobv("1111000011001100")));
                }
              //for (unsigned i = 2; i < words.size(); i ++)
                {
                  *logs << "+TestBlif Added44words..." << "I0=$false" << " \n";
                  const std::string &w = "I0=$false";
                  std::size_t p = w.find('=');
                  if (p == std::string::npos)
                    fatal("invalid formal-actual");
                  
                  std::string formal(w, 0, p),
                    actual(w, p+1);
                  
                  Port *port = inst->find_port(formal);
                  if (!port)
                    fatal(fmt("unknown formal `" << formal << "'"));
                  
                  Net *net = top->find_or_add_net(actual);
                  port->connect(net);
                }
                {
                  *logs << "+TestBlif Added44words..." << "I1=key.r0[1]" << " \n";
                  const std::string &w = "I1=key.r0[1]";
                  std::size_t p = w.find('=');
                  if (p == std::string::npos)
                    fatal("invalid formal-actual");
                  
                  std::string formal(w, 0, p),
                    actual(w, p+1);
                  
                  
                  Port *port = inst->find_port(formal);
                  if (!port)
                    fatal(fmt("unknown formal `" << formal << "'"));
                  
                  Net *net = top->find_or_add_net(actual);
                  port->connect(net);
                }
                {
                  *logs << "+TestBlif Added44words..." << "I2=d_out1[1]" << " \n";
                  const std::string &w = "I2=d_out1[1]";
                  std::size_t p = w.find('=');
                  if (p == std::string::npos)
                    fatal("invalid formal-actual");
                  
                  std::string formal(w, 0, p),
                    actual(w, p+1);
                  
                  
                  Port *port = inst->find_port(formal);
                  if (!port)
                    fatal(fmt("unknown formal `" << formal << "'"));
                  
                  Net *net = top->find_or_add_net(actual);
                  port->connect(net);
                }
                {
                  *logs << "+TestBlif Added44words..." << "I3=s" << " \n";
                  const std::string &w = "I3=s";
                  std::size_t p = w.find('=');
                  if (p == std::string::npos)
                    fatal("invalid formal-actual");
                  
                  std::string formal(w, 0, p),
                    actual(w, p+1);
                  
                  
                  Port *port = inst->find_port(formal);
                  if (!port)
                    fatal(fmt("unknown formal `" << formal << "'"));
                  
                  Net *net = top->find_or_add_net(actual);
                  port->connect(net);
                }
                {
                  *logs << "+TestBlif Added44words..." << "O=d_out[1]" << " \n";
                  const std::string &w = "O=d_out[1]";
                  std::size_t p = w.find('=');
                  if (p == std::string::npos)
                    fatal("invalid formal-actual");
                  
                  std::string formal(w, 0, p),
                    actual(w, p+1);
                  
                  
                  Port *port = inst->find_port(formal);
                  if (!port)
                    fatal(fmt("unknown formal `" << formal << "'"));
                  
                  Net *net = top->find_or_add_net(actual);
                  port->connect(net);
                }
                //Test d_out[2]
              const std::string &n2 = "SB_LUT4";
              *logs << "+TestBlif Added44..." << "SB_LUT4" << " \n";

              Model *inst_of_2 = d->find_model(n2);
              
              inst = top->add_instance(inst_of_2);
              std::string ss2 = "\"./usr/local/bin/../share/yosys/ice40/cells_map.v:22.34-23.52\"";
              *logs << "+TestAttr Added11..." << " words1 " << "src" << " words2 " << ss2 << " \n";
                assert(ss2.back() == '"');
                  inst->set_attr("src", 
                                 Const(lp, ss2.substr(1, ss2.size() - 2)));

                {*logs << "+TestAttr Added22..." << " words1 " << "module_not_derived" << " words2 " << "00000000000000000000000000000001" << " \n";
                  inst->set_attr("module_not_derived",
                                 Const(lp, stobv("00000000000000000000000000000001")));
                }

                {*logs << "+TestBlif Added66..." << "LUT_INIT"  << " " << "1111000011001100" << " \n";
                inst->set_param("LUT_INIT",
                                Const(lp, stobv("1111000011001100")));
                }
              //for (unsigned i = 2; i < words.size(); i ++)
                {
                  *logs << "+TestBlif Added44words..." << "I0=$false" << " \n";
                  const std::string &w = "I0=$false";
                  std::size_t p = w.find('=');
                  if (p == std::string::npos)
                    fatal("invalid formal-actual");
                  
                  std::string formal(w, 0, p),
                    actual(w, p+1);
                  
                  Port *port = inst->find_port(formal);
                  if (!port)
                    fatal(fmt("unknown formal `" << formal << "'"));
                  
                  Net *net = top->find_or_add_net(actual);
                  port->connect(net);
                }
                {
                  *logs << "+TestBlif Added44words..." << "I1=key.r0[2]" << " \n";
                  const std::string &w = "I1=key.r0[2]";
                  std::size_t p = w.find('=');
                  if (p == std::string::npos)
                    fatal("invalid formal-actual");
                  
                  std::string formal(w, 0, p),
                    actual(w, p+1);
                  
                  
                  Port *port = inst->find_port(formal);
                  if (!port)
                    fatal(fmt("unknown formal `" << formal << "'"));
                  
                  Net *net = top->find_or_add_net(actual);
                  port->connect(net);
                }
                {
                  *logs << "+TestBlif Added44words..." << "I2=d_out1[2]" << " \n";
                  const std::string &w = "I2=d_out1[2]";
                  std::size_t p = w.find('=');
                  if (p == std::string::npos)
                    fatal("invalid formal-actual");
                  
                  std::string formal(w, 0, p),
                    actual(w, p+1);
                  
                  
                  Port *port = inst->find_port(formal);
                  if (!port)
                    fatal(fmt("unknown formal `" << formal << "'"));
                  
                  Net *net = top->find_or_add_net(actual);
                  port->connect(net);
                }
                {
                  *logs << "+TestBlif Added44words..." << "I3=s" << " \n";
                  const std::string &w = "I3=s";
                  std::size_t p = w.find('=');
                  if (p == std::string::npos)
                    fatal("invalid formal-actual");
                  
                  std::string formal(w, 0, p),
                    actual(w, p+1);
                  
                  
                  Port *port = inst->find_port(formal);
                  if (!port)
                    fatal(fmt("unknown formal `" << formal << "'"));
                  
                  Net *net = top->find_or_add_net(actual);
                  port->connect(net);
                }
                {
                  *logs << "+TestBlif Added44words..." << "O=d_out[2]" << " \n";
                  const std::string &w = "O=d_out[2]";
                  std::size_t p = w.find('=');
                  if (p == std::string::npos)
                    fatal("invalid formal-actual");
                  
                  std::string formal(w, 0, p),
                    actual(w, p+1);
                  
                  
                  Port *port = inst->find_port(formal);
                  if (!port)
                    fatal(fmt("unknown formal `" << formal << "'"));
                  
                  Net *net = top->find_or_add_net(actual);
                  port->connect(net);
                }
                //Test d_out[3]
              const std::string &n3 = "SB_LUT4";
              *logs << "+TestBlif Added44..." << "SB_LUT4" << " \n";

              Model *inst_of_3 = d->find_model(n3);
              
              inst = top->add_instance(inst_of_3);
              std::string ss3 = "\"./usr/local/bin/../share/yosys/ice40/cells_map.v:22.34-23.52\"";
              *logs << "+TestAttr Added11..." << " words1 " << "src" << " words2 " << ss3 << " \n";
                assert(ss3.back() == '"');
                  inst->set_attr("src", 
                                 Const(lp, ss3.substr(1, ss3.size() - 2)));

                {*logs << "+TestAttr Added22..." << " words1 " << "module_not_derived" << " words2 " << "00000000000000000000000000000001" << " \n";
                  inst->set_attr("module_not_derived",
                                 Const(lp, stobv("00000000000000000000000000000001")));
                }

                {*logs << "+TestBlif Added66..." << "LUT_INIT"  << " " << "1111000011001100" << " \n";
                inst->set_param("LUT_INIT",
                                Const(lp, stobv("1111000011001100")));
                }
              //for (unsigned i = 2; i < words.size(); i ++)
                {
                  *logs << "+TestBlif Added44words..." << "I0=$false" << " \n";
                  const std::string &w = "I0=$false";
                  std::size_t p = w.find('=');
                  if (p == std::string::npos)
                    fatal("invalid formal-actual");
                  
                  std::string formal(w, 0, p),
                    actual(w, p+1);
                  
                  Port *port = inst->find_port(formal);
                  if (!port)
                    fatal(fmt("unknown formal `" << formal << "'"));
                  
                  Net *net = top->find_or_add_net(actual);
                  port->connect(net);
                }
                {
                  *logs << "+TestBlif Added44words..." << "I1=key.r0[3]" << " \n";
                  const std::string &w = "I1=key.r0[3]";
                  std::size_t p = w.find('=');
                  if (p == std::string::npos)
                    fatal("invalid formal-actual");
                  
                  std::string formal(w, 0, p),
                    actual(w, p+1);
                  
                  
                  Port *port = inst->find_port(formal);
                  if (!port)
                    fatal(fmt("unknown formal `" << formal << "'"));
                  
                  Net *net = top->find_or_add_net(actual);
                  port->connect(net);
                }
                {
                  *logs << "+TestBlif Added44words..." << "I2=d_out1[3]" << " \n";
                  const std::string &w = "I2=d_out1[3]";
                  std::size_t p = w.find('=');
                  if (p == std::string::npos)
                    fatal("invalid formal-actual");
                  
                  std::string formal(w, 0, p),
                    actual(w, p+1);
                  
                  
                  Port *port = inst->find_port(formal);
                  if (!port)
                    fatal(fmt("unknown formal `" << formal << "'"));
                  
                  Net *net = top->find_or_add_net(actual);
                  port->connect(net);
                }
                {
                  *logs << "+TestBlif Added44words..." << "I3=s" << " \n";
                  const std::string &w = "I3=s";
                  std::size_t p = w.find('=');
                  if (p == std::string::npos)
                    fatal("invalid formal-actual");
                  
                  std::string formal(w, 0, p),
                    actual(w, p+1);
                  
                  
                  Port *port = inst->find_port(formal);
                  if (!port)
                    fatal(fmt("unknown formal `" << formal << "'"));
                  
                  Net *net = top->find_or_add_net(actual);
                  port->connect(net);
                }
                {
                  *logs << "+TestBlif Added44words..." << "O=d_out[3]" << " \n";
                  const std::string &w = "O=d_out[3]";
                  std::size_t p = w.find('=');
                  if (p == std::string::npos)
                    fatal("invalid formal-actual");
                  
                  std::string formal(w, 0, p),
                    actual(w, p+1);
                  
                  
                  Port *port = inst->find_port(formal);
                  if (!port)
                    fatal(fmt("unknown formal `" << formal << "'"));
                  
                  Net *net = top->find_or_add_net(actual);
                  port->connect(net);
                }     
              goto M;
            }
          else
            fatal(fmt("unknown directive '" << cmd << "'"));
        }
      else
        fatal("expected directive");
    }
  
 M:

  if (!top)
    fatal("no top model has been defined");

  // unify
  std::map<Net *, Net *, IdLess> replacement;
  for (const auto &p : unify)
    {
      // n1 drives n2
      Net *n1 = p.first,
        *n2 = p.second;
      
      Net *r = n1;
      while (Net *t = lookup_or_default(replacement, r, nullptr))
        r = t;
      
      Net *x = n1;
      while (x != r)
        {
          auto i = replacement.find(x);
          assert(i != replacement.end());
          
          x = i->second;
          i->second = r;
        }
      
      if (n2 == r)
        fatal(".names cycle\n");
      
      n2->replace(r);

      if (contains(replacement, n2))
        fatal("conflicting .names outputs");

      extend(replacement, n2, r);
    }
  for (const auto &p : replacement)
    {
      Net *n = p.first;
      top->remove_net(n);
      delete n;
    }
  
  // Replace SB_IO_ODs with inconsistent naming with SB_IO_OD_As with sensible
  // naming for internal use
  std::vector<Instance *> io_od_to_rep;
  
  for (auto od_i : top->instances())
    {
      if(od_i->instance_of() == io_od_model)
        io_od_to_rep.push_back(od_i);
    }
  
  for (auto od_i : io_od_to_rep)
    {
      Instance *od_a_inst = top->add_instance(io_od_a_model);
      for (auto port : od_a_inst->ports())
        {
          std::string sb_name;
          for (auto chr : port.first)
            if (chr != '_')
               sb_name += chr;
          od_a_inst->find_port(port.first)->connect(od_i->find_port(sb_name)->connection());
        }
      for (auto param : od_i->params())
        {
          od_a_inst->set_param(param.first, od_i->get_param(param.first));
        }
      od_i->remove();
      delete od_i;
    }
  
  Models models(d);
  std::map<Port *, Direction> inout_redir;
  for (const auto &p : top->ports())
    {
      if (p.second->is_bidir())
        {
          Net *n = p.second->connection();
          if (n)
            {
              bool has_input = false, has_output = false, has_tristate = false;
              for (const auto &q : n->connections())
                {
                  if (q == p.second)
                    continue;

                  if (q->is_input())
                    has_input = true;

                  if (q->is_output())
                    has_output = true;

                  if (isa<Instance>(q->node())
                      && ((models.is_tbuf(cast<Instance>(q->node()))
                          && q->name() == "Y")
                      || (models.is_ioX(cast<Instance>(q->node()))
                          && q->name() == "PACKAGE_PIN")
                      || (models.is_pllX(cast<Instance>(q->node()))
                          && q->name() == "PACKAGEPIN")
                      || (models.is_rgba_drv(cast<Instance>(q->node())) 
                        &&  (q->name() == "RGB0" || q->name() == "RGB1" || q->name() == "RGB2"))))
                    has_tristate = true;
                }

              // inout net has a tristate driver and used as an input, illegal
              if (has_input && has_tristate)
                  fatal(fmt("toplevel inout port '" << p.second->name()
                            << "' connected to tristate buffer and driving a net"));

              // inout net used as output
              else if (has_output && !has_tristate)
                inout_redir[p.second] = Direction::OUT;

              // inout net used as input
              else if (!has_output)
                inout_redir[p.second] = Direction::IN;

              // inout net unused
              else if (n->connections().size() == 1)
                inout_redir[p.second] = Direction::IN;
            }
        }
    }

  for (const auto &p : inout_redir)
    p.first->set_direction(p.second);
  
  std::set<Net *, IdLess> boundary_nets;
  for (Instance *inst2 : top->instances())
    {
      if (inst2->instance_of() == io_model)
        {
          Port *p = inst2->find_port("PACKAGE_PIN");
          Net *n = p->connection();
          Port *q = p->connection_other_port();
          if (!n
              || !q
              || !isa<Model>(q->node()))
            fatal("SB_IO PACKAGE_PIN not connected to toplevel port");
          
          extend(boundary_nets, n);
        }
    }
  
  for (const auto &p : top->nets())
    {
      Net *n = p.second;
      if (contains(boundary_nets, n))
        continue;
      
      int n_drivers = 0;
      if (n->is_constant())
        ++n_drivers;
      for (Port *p2 : n->connections())
        {
          if (p2->is_output())
            ++n_drivers;
        }
      if (n_drivers > 1)
        fatal(fmt("net `" << n->name() << "' has multiple drivers"));
    }
  
  return d;
}

Design *
read_blif(const std::string &filename)
{
  std::string expanded = expand_filename(filename);
  std::ifstream fs(expanded);
  if (fs.fail())
    fatal(fmt("read_blif: failed to open `" << expanded << "': "
              << strerror(errno)));
  BlifParser parser(filename, fs);
  return parser.parse();
}

Design *
read_blif(const std::string &filename, std::istream &s)
{
  BlifParser parser(filename, s);
  return parser.parse();
}
