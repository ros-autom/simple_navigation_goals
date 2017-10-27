int BoxCount() {

  int boxCounter = 0;
  // grab the home dir
  struct passwd *pw = getpwuid(getuid());
  const char *homedir = pw->pw_dir;
  std::string ss2 = "/warehouse_items.txt";
  std::string ss3 = homedir + ss2;

  std::ifstream input_stream(ss3, std::ios::in);
  if (!input_stream.is_open()) {
    ROS_ERROR("Failed to open the warehouse_items.txt");
    return -1;
  }

  for (std::string line; getline(input_stream, line);)
    if (line.find("_") != std::string::npos)
      boxCounter++;

  input_stream.close();
  return boxCounter;
}

void PrintProducts(Box *products_, int size_) {

  int qr = 0, cola = 0, fanta = 0, lays = 0;
  for (int i = 0; i < size_; i++) {
    if ((products_[i].GetType().find("CocaCola") != std::string::npos) && !products_[i].GetDevStatus())
      cola++;
    else if ((products_[i].GetType().find("Fanta") != std::string::npos)  && !products_[i].GetDevStatus())
      fanta++;
    else if ((products_[i].GetType().find("Lays") != std::string::npos)  && !products_[i].GetDevStatus())
      lays++;
    else if ((products_[i].GetType().find("Qr") != std::string::npos)  && !products_[i].GetDevStatus())
      qr++;
  }
  if(cola==0 && qr==0 && fanta==0 && lays==0)
  {
    ROS_ERROR("There are no more boxes in the warehouse");
    delete [] Boxes;
    std::exit(0);
  }
	std::cout<<"_________________"<<std::endl;
	std::cout<<"Coca Cola boxes: "<<cola<<std::endl;
	std::cout<<"Fanta boxes: "<<fanta<<std::endl;
	std::cout<<"Lays boxes: "<<lays<<std::endl;
	std::cout<<"QR boxes: "<<qr<<std::endl;
	std::cout<<"_________________"<<std::endl;
}
//count the remaining boxes in the warehouse (for the service)
int CountProducts(Box *products_,int size_,std::string typeParam_)
{
  int typeCounter=0;
  for(int i=0 ; i<size_;i++)
    if ((products_[i].GetType().find(typeParam_) != std::string::npos) && !products_[i].GetDevStatus())
      typeCounter++;

  return typeCounter;
}
