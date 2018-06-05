#include "cJSON.c"
extern char *config_get(char *name);

cJSON *dofile()
{
	char *data;
	FILE *fp;
	int len;
	cJSON *root;
	fp = fopen("/tmp/openvpn/client/configs/providerlist.json", "r");
	if(fp == NULL)
		goto err;
	fseek(fp,0,SEEK_END);
	len = ftell(fp);
	fseek(fp,0,SEEK_SET);
	data = (char *)malloc((len + 1)*sizeof(char));
	fread(data,1,len,fp);
	data[len] = '\0';
	root = cJSON_Parse(data);
	free(data);
	data = NULL;
	fclose(fp);
	return root;
err:
	fprintf(stderr, "openvpn client: open json file fail!\n");
	exit(1);
}
	
void print_anycity(cJSON *Object)
{
	FILE *fp;
	fp = fopen("/tmp/openvpn_anycity","w+");
	cJSON *CityList;
	CityList = cJSON_GetObjectItem(Object,"cityList");
	char *s = cJSON_Print(CityList);
	int len = cJSON_GetArraySize(CityList);
	int i;
	cJSON *print;
	for(i=0;i<len;i++)
	{
		print = cJSON_GetArrayItem(CityList,i);
		fprintf(fp,"%s\n",print->valuestring);
	}
	fclose(fp);	
}
void get_city_list(cJSON *Object)
{
	cJSON *Country_List = cJSON_GetObjectItem(Object,"countryList");
	int country_num = cJSON_GetArraySize(Country_List);
	int i;
	char *cfg_country = config_get("vpn_client_ovpn_cfg_country");
	cJSON *Country_Object,*Country_name;
	for(i=0;i<country_num;i++)
	{
		Country_Object = cJSON_GetArrayItem(Country_List,i);
		Country_name = cJSON_GetObjectItem(Country_Object,"countryName");
		if(!strcmp(Country_name->valuestring,cfg_country))
		{
			print_anycity(Country_Object);
			break;
		}
	}
}

void get_country_list(cJSON *Object)
{
	cJSON *Provider_List,*Provider_Object,*Provider_name;
	char *cfg_provider = config_get("vpn_client_ovpn_cfg_provider");
	int provider_num,i;
	Provider_List = cJSON_GetObjectItem(Object,"providerList");
	provider_num = cJSON_GetArraySize(Provider_List);

	for(i = 0 ; i < provider_num ; i++)
	{
		Provider_Object = cJSON_GetArrayItem(Provider_List,i);
		Provider_name = cJSON_GetObjectItem(Provider_Object,"providerName");
		if( !strcmp(Provider_name->valuestring,cfg_provider))
		{
			get_city_list(Provider_Object);
			break;
		}
		else continue;
	}
}
void main()
{
	cJSON *Root;
	Root = dofile();
	get_country_list(Root);
	cJSON_Delete(Root);
	return;
}
