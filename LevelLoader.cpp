#include "LevelLoader.h"
#include "TextureLoader.h"
#include "Position.h"
#include "Sprite.h"
#include "Player.h"
#include "Ball.h"
#include "Brick.h"
#include "Active.h"
#include "CollisionAABB.h"
#include "CollisionCircle.h"
#include "PaddleCollisionObj.h"

#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/encodedstream.h"
#include "rapidjson/writer.h"

#include <entt/entt.hpp>
#include <errno.h>
#include <stdexcept>

using namespace SDLEngine;
using namespace Breakout;

static constexpr char BACKGROUND[] = "background";
static constexpr char PADDLE[]     = "paddle";
static constexpr char BALL[]       = "ball";
static constexpr char BRICKS[]     = "bricks";
static constexpr char BRICK_X[]     = "x";
static constexpr char BRICK_Y[]     = "y";
static constexpr char BRICK_COLOR[] = "color";
static constexpr char MUSIC[]      = "music";

static constexpr char PADDLE_ASSET[] = "Assets/Graphics/Paddle/paddleBlu.png";
static constexpr char BALL_ASSET[] = "Assets/Graphics/Ball/ballGrey.png";

void LevelLoader::LoadBrickSprites(LevelBreakout &level, SDL_Renderer* renderer)
{
	const int nBrickCount = static_cast<int>(Brick::BrickType::Yellow) + 1;
	for(int i = 0; i < nBrickCount; ++i)
	{
		level.BrickTextures[i] = TextureLoader::LoadFromFile(renderer, GetBrickPath(Brick::BrickType(i)), 1);
	}
	
}

LevelBreakout LevelLoader::LoadLevel(std::string filename, entt::registry& reg, SDL_Renderer *renderer)
{
	LevelBreakout ret;

	LoadBrickSprites(ret, renderer);

	char readBuffer[65536];
	FILE* fp = nullptr;
	auto err = fopen_s(&fp, filename.c_str(), "rb");
	if (err == 0)
	{
		rapidjson::FileReadStream bis(fp, readBuffer, sizeof(readBuffer));
		rapidjson::AutoUTFInputStream<unsigned, rapidjson::FileReadStream> eis(bis);

		rapidjson::Document doc;
		doc.ParseStream<0, rapidjson::AutoUTF<unsigned>>(eis);

		if (!doc.HasParseError())
		{
			const auto& background = doc[BACKGROUND];
			ret.BackgroundEntity = reg.create();
			reg.emplace<Position>(ret.BackgroundEntity, 0.0f, 0.0f);
			reg.emplace<Sprite>(ret.BackgroundEntity, TextureLoader::LoadFromFile(renderer, background.GetString(), 0));
			reg.emplace<Active>(ret.BackgroundEntity);

			ret.PaddleEntity = reg.create();
			reg.emplace<Position>(ret.PaddleEntity, 0.0f, 0.0f);
			auto paddleSprite = TextureLoader::LoadFromFile(renderer, PADDLE_ASSET, 1);
			int paddleHalfHeight = paddleSprite.Rect.h / 2;
			auto paddleCenterEntity = reg.create();
			reg.emplace<CollisionAABB>  (
				paddleCenterEntity,
				paddleSprite.Rect.w - paddleHalfHeight * 2.0f, 
				static_cast<float>(paddleSprite.Rect.h), 
				glm::vec2(static_cast<float>(paddleHalfHeight), 0.0f));
			auto paddleLeftEntity = reg.create();
			reg.emplace<CollisionCircle>(paddleLeftEntity, static_cast<float>(paddleHalfHeight), glm::vec2(paddleHalfHeight, paddleHalfHeight));
			auto paddleRightEntity = reg.create();
			reg.emplace<CollisionCircle>(paddleRightEntity, static_cast<float>(paddleHalfHeight), glm::vec2(paddleSprite.Rect.w-paddleHalfHeight, paddleHalfHeight));
			reg.emplace<PaddleCollisionObj>(ret.PaddleEntity, paddleCenterEntity , paddleLeftEntity, paddleRightEntity);
			reg.emplace<Sprite>(ret.PaddleEntity, std::move(paddleSprite));
			reg.emplace<Player>(ret.PaddleEntity);
			reg.emplace<Active>(ret.PaddleEntity);

			ret.BallEntity = reg.create();
			reg.emplace<Position>(ret.BallEntity, 0.0f, 0.0f);
			auto ballTexture = TextureLoader::LoadFromFile(renderer, BALL_ASSET, 1);
			reg.emplace<CollisionCircle>(ret.BallEntity, ballTexture.Rect.w / 2.0f, glm::vec2(ballTexture.Rect.w / 2.0f, ballTexture.Rect.h / 2.0f));
			reg.emplace<Sprite>(ret.BallEntity, std::move(ballTexture));
			reg.emplace<Ball>(ret.BallEntity);
			reg.emplace<Active>(ret.BallEntity);

			const auto& music = doc[MUSIC];
			ret.LevelMusicPath = music.GetString();

			const rapidjson::Value& bricks = doc[BRICKS];
			ret.BricksEntities.reserve(bricks.Size());
			for (rapidjson::Value::ConstValueIterator itr = bricks.Begin(); itr != bricks.End(); ++itr)
			{
				rapidjson::Value::ConstMemberIterator x = itr->FindMember(BRICK_X);
				rapidjson::Value::ConstMemberIterator y = itr->FindMember(BRICK_Y);
				rapidjson::Value::ConstMemberIterator color = itr->FindMember(BRICK_COLOR);

				auto brick = reg.create();
				ret.BricksEntities.push_back(brick);
				reg.emplace<Position>(brick, static_cast<float>(x->value.GetInt()), static_cast<float>(y->value.GetInt()));
				auto brickTexture = ret.GetBrickSprite(Brick::BrickType(color->value.GetInt()));
				reg.emplace<CollisionAABB>(brick, static_cast<float>(brickTexture.Rect.w), static_cast<float>(brickTexture.Rect.h), glm::vec2());
				reg.emplace<Sprite>(brick, std::move(brickTexture));
				reg.emplace<Brick>(brick);
				reg.emplace<Active>(brick);
			}
		}
		else
			throw std::runtime_error("Parse error while reading level file : " + filename);
	}
	else
		throw  std::runtime_error("Failed to open level file");

	if (fp)
	{
		if(fclose(fp) != 0)
			throw  std::runtime_error("Failed to close level file");
	}

	return ret;
}

void LevelLoader::SaveLevel(std::string filename, const LevelAssets& level, entt::registry& reg)
{
	char writeBuffer[65536];
	FILE* fp = nullptr;
	auto err = fopen_s(&fp, filename.c_str(), "wb");
	if (err == 0)
	{
		rapidjson::Document doc;
		doc.SetObject();
		auto& allocator = doc.GetAllocator();

		rapidjson::Value backgroundValue;
		backgroundValue.SetString(level.Background.c_str(), static_cast<rapidjson::SizeType>(level.Background.length()), allocator);
		doc.AddMember(BACKGROUND, backgroundValue, allocator);

		rapidjson::Value musicValue;
		musicValue.SetString(level.Music.c_str(), static_cast<rapidjson::SizeType>(level.Music.length()), allocator);
		doc.AddMember(MUSIC, musicValue, allocator);

		rapidjson::Value brickArray(rapidjson::kArrayType);
		
		for (const auto& brick : level.Bricks)
		{
			rapidjson::Value brickObj;
			brickObj.SetObject();
			brickObj.AddMember(BRICK_X    , brick.PosX, allocator);
			brickObj.AddMember(BRICK_Y    , brick.PosY, allocator);
			brickObj.AddMember(BRICK_COLOR, static_cast<int>(brick.Type), allocator);
			brickArray.PushBack(brickObj, allocator);
		}
		doc.AddMember(BRICKS, brickArray, allocator);

		rapidjson::FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));
		rapidjson::Writer writer(os);

		doc.Accept(writer);
;	}
	else
		throw  std::runtime_error("Failed to save level file");

	if (fp)
	{
		if (fclose(fp) != 0)
			throw  std::runtime_error("Failed to close level file");
	}
}

